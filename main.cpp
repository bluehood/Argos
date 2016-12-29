#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <stdlib.h>

/** We need this to easily convert between pixel and real-world coordinates*/
static const float SCALE = 32.f;
static const float GRAVITY = 9.8f * 2;

class Logger {
  bool newLog = true;
public:
  Logger() {
  }

  Logger& operator<<(const std::string& s) {
    if (newLog) {
      newLog = false;
      std::cout << "[LOG] ";
    }
    std::cout << s;
    return *this;
  }

  void operator<<(Logger&) {
    newLog = true;
    std::cout << std::endl;
  }
};

Logger mainLogger;

class TileSet {
  sf::Texture texture;
  std::unordered_map<std::string, sf::Sprite > Sprites;
public:
  TileSet(const std::string& basePath) {
    texture.loadFromFile(basePath + ".png");
    std::ifstream infile(basePath + ".dat");
    std::string id;
    int x, y, wx, wy;

    while (infile >> id >> x >> y >> wx >> wy) {
      sf::Sprite sprite(texture);
      sprite.setTextureRect({x, y, wx, wy});
      Sprites[id] = sprite;
    }
  }

  bool contains(const std::string& id) const {
    return Sprites.find(id) != Sprites.end();
  }

  sf::Sprite& operator[](const std::string& id) {
    return Sprites[id];
  }
};

class TileData {

  bool passable_ = false;
  sf::Sprite sprite_;
  std::string name_;
  std::string baseName_;

public:
  TileData(const std::string& name, const std::string& baseName)
      : name_(name), baseName_(baseName) {
  }

  const std::string& baseName() const {
    return baseName_;
  }

  const std::string& name() const {
    return name_;
  }

  void passable(bool v) {
    passable_ = v;
  }

  bool passable() const {
    return passable_;
  }

  void sprite(const sf::Sprite& s) {
    sprite_ = s;
  }

  const sf::Sprite& sprite() const {
    return sprite_;
  }
};

class GameData {

  std::vector<TileSet*> TileSets;
  std::unordered_map<std::string, TileData*> Tiles;

  const sf::Sprite& getSprite(const std::string& id) {
    for (TileSet* ts : TileSets) {
      if (ts->contains(id)) {
        return (*ts)[id];
      }
    }
    assert(false);
  }

  void addTile(const std::string& name, const std::string& baseName, bool passable = false) {
    Tiles[name] = new TileData(name, baseName);
    Tiles[name]->sprite(getSprite(name));
    Tiles[name]->passable(passable);
  }

  void addMetaTile(const std::string& name, bool passable = false) {
    addTile(name + "_ul", name, passable);
    addTile(name + "_u", name, passable);
    addTile(name + "_ur", name, passable);
    addTile(name + "_l", name, passable);
    addTile(name, name, passable);
    addTile(name + "_r", name, passable);
    addTile(name + "_bl", name, passable);
    addTile(name + "_b", name, passable);
    addTile(name + "_br", name, passable);
    addTile(name + "_wvu", name, passable);
    addTile(name + "_wv", name, passable);
    addTile(name + "_wvb", name, passable);
    addTile(name + "_whl", name, passable);
    addTile(name + "_wh", name, passable);
    addTile(name + "_whr", name, passable);
    addTile(name + "_f1", name, passable);
    addTile(name + "_f2", name, passable);
  }

public:
  GameData(const std::string& path) {
    parseMetaFile(path);
    addMetaTile("cave");
  }

  virtual ~GameData() {
    for (TileSet* s : TileSets)
      delete s;
    for (auto& s : Tiles)
      delete s.second;
  }

  TileData* operator[](const std::string& id) {
    auto I = Tiles.find(id);
    if (I != Tiles.end())
      return I->second;
    std::cerr << "Couldn't find " << id << std::endl;
    assert(false);
  }

  void parseMetaFile(const std::string& path) {
    mainLogger << "Parsing " << path << "/meta.dat" << mainLogger;
    std::ifstream input(path + "/meta.dat");
    std::string fileType;
    std::string filePath;
    while (input >> fileType >> filePath) {
      filePath = path + "/" + filePath;
      if (fileType == "meta") {
        parseMetaFile(filePath);
      } else if (fileType == "tiles") {
        mainLogger << "Loading tile set " << filePath << mainLogger;
        TileSets.push_back(new TileSet(filePath));
      } else {
        mainLogger << "Unknown file type in meta file " << path << "/meta.dat: "
                  << mainLogger;
      }
    }
  }
};

class GameObject {
public:
  virtual void update() {}
  virtual void render(sf::RenderTarget& target) {}
};

class Tile : public GameObject {
  b2Body* Body = nullptr;

  TileData* Data = nullptr;

  void destroyBody(b2World& World) {
    if (Body) {
      World.DestroyBody(Body);
      Body = nullptr;
    }
  }

public:
  Tile() {}

  void setData(b2World& world, TileData* d) {
    Data = d;
    if (!d->passable()) {
      createBody(world);
    }
  }

  const std::string& name() const {
    if (Data)
      return Data->name();
    else {
      static std::string noName;
      return noName;
    }
  }

  const std::string& baseName() const {
    if (Data)
      return Data->baseName();
    else {
      static std::string noName;
      return noName;
    }
  }

  bool hasCliff() const {
    if (name().size() < 2)
      return false;
    if (name()[name().size() - 2] == 'u')
      return true;
    if (name()[name().size() - 2] == 'f')
      return true;
    if (name()[name().size() - 2] == 'v' && name()[name().size() - 1] == 'u')
      return true;
    if (name()[name().size() - 2] == 'h' && name()[name().size() - 1] == 'l')
      return true;
    if (name()[name().size() - 2] == 'h' && name()[name().size() - 1] == 'r')
      return true;

    return false;
  }

  void createBody(b2World& World) {
    destroyBody(World);

    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x, y);
    BodyDef.type = b2_staticBody;
    BodyDef.fixedRotation = true;
    Body = World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 10.f;
    FixtureDef.friction = 0.9f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
    Body->SetUserData((void*)this);
  }

  size_t x, y;

  bool empty() {
    return !Data;
  }

  void render(sf::RenderTarget& target) {
    if (!Body)
      return;
    if (!Data)
      return;

    sf::Sprite sprite = Data->sprite();

    sprite.setOrigin(16.f, 16.f);
    sprite.setPosition(SCALE * Body->GetPosition().x,
                       SCALE * Body->GetPosition().y);
    sprite.setRotation(Body->GetAngle() * 180 / b2_pi);
    target.draw(sprite);
  }

};

class Level {

  GameData& data;
  std::vector<GameObject *> Objects;
  std::vector<Tile> Tiles;
  size_t w;
  size_t h;

  sf::Texture background;

  std::vector<int> surroundings(int x, int y) {
    std::string n = get(x, y).baseName();
    return {
        get(x-1,y-1).baseName() == n ? 1 : 0,
        get(x  ,y-1).baseName() == n ? 1 : 0,
        get(x+1,y-1).baseName() == n ? 1 : 0,
        get(x-1,y  ).baseName() == n ? 1 : 0,
        get(x+1,y  ).baseName() == n ? 1 : 0,
        get(x-1,y+1).baseName() == n ? 1 : 0,
        get(x  ,y+1).baseName() == n ? 1 : 0,
        get(x+1,y+1).baseName() == n ? 1 : 0
    };
  }

  bool compatible(std::vector<int> is, std::vector<int> expected) {
    assert(is.size() == 8);
    assert(is.size() == expected.size());
    for (size_t i = 0; i < is.size(); i++) {
      if (expected[i] == 2)
        continue;
      if (expected[i] != is[i])
        return false;
    }
    return true;
  }

  std::string getSuffix(std::vector<int> s) {
    if (compatible(s, {2, 0, 2, 0, 1, 2, 1, 2}))
      return "_ul";
    if (compatible(s, {2, 0, 2, 1, 1, 2, 1, 2}))
      return "_u";
    if (compatible(s, {2, 0, 2, 1, 0, 2, 1, 2}))
      return "_ur";

    if (compatible(s, {2, 1, 2, 1, 1, 2, 0, 2}))
      return "_b";

    if (compatible(s, {2, 1, 2, 0, 1, 2, 0, 2}))
      return "_bl";
    if (compatible(s, {2, 1, 2, 1, 0, 2, 0, 2}))
      return "_br";

    if (compatible(s, {2, 1, 2, 0, 1, 2, 1, 2}))
      return "_l";

    if (compatible(s, {2, 1, 2, 1, 0, 2, 1, 2}))
      return "_r";

    if (compatible(s, {2, 1, 2, 0, 0, 2, 1, 2}))
      return "_wv";
    if (compatible(s, {2, 0, 2, 0, 0, 2, 0, 2})) {
      if (rand() % 2)
        return "_f1";
      else
        return "_f2";
    }
    if (compatible(s, {2, 0, 2, 0, 0, 2, 1, 2}))
      return "_wvu";
    if (compatible(s, {2, 1, 2, 0, 0, 2, 0, 2}))
      return "_wvb";
    if (compatible(s, {2, 0, 2, 0, 1, 2, 0, 2}))
      return "_whl";
    if (compatible(s, {2, 0, 2, 1, 0, 2, 0, 2}))
      return "_whr";
    if (compatible(s, {2, 0, 2, 1, 1, 2, 0, 2}))
      return "_wh";
    return "";
  }

  void levelGen() {
    for (size_t x = 0; x < w; ++x) {
      for (size_t y = 0; y < h; ++y) {
        if (rand() % 100 < 30)
          get(x, y).setData(World, data["cave"]);
      }
    }

    for (size_t x = 0; x < w; ++x) {
      for (size_t y = 0; y < h; ++y) {
        if (!get(x, y).empty()) {
          std::string name = get(x, y).name();
          get(x, y).setData(World, data[name + getSuffix(surroundings(x, y))]);
        }
      }
    }
  }

  Tile dummyTile;

public:
  Level(size_t w, size_t h, GameData& data) : data(data), World({0, GRAVITY}), w(w), h(h) {
    background.loadFromFile("background.png");
    background.setRepeated(true);
    Tiles.resize(w * h);
    for (size_t x = 0; x < w; ++x) {
      for (size_t y = 0; y < h; ++y) {
        Tiles[x + y * w].x = x;
        Tiles[x + y * w].y = y;
      }
    }
    levelGen();
  }
  b2World World;

  class HasContact : public b2ContactListener {
    void BeginContact(b2Contact* contact) {
    }

    void EndContact(b2Contact* contact) {
    }
  };

  HasContact hasContact;
  double time = 0;

  Tile& get(int x, int y) {
    if (x < 0 || x >= w || y < 0 || y >= h) {
      dummyTile = Tile();
      return dummyTile;
    }
    return Tiles[x + y * w];
  }

  void update() {
    time += 1 / 60.0;
    for (GameObject* o : Objects)
      o->update();
  }

  void add(GameObject* o) {
    Objects.push_back(o);
  }

  void render(sf::RenderTarget& target) {
    sf::Sprite back(background);
    back.setTextureRect({0, 0, target.getView().getSize().x, target.getView().getSize().y});
    target.draw(back);

    for (Tile& t : Tiles)
      t.render(target);
    for (GameObject* o : Objects)
      o->render(target);
  }
};

class Character : public GameObject {

  bool looksRight = true;
  b2Body* Body;
  sf::Texture texture;
  Level* level_;

  bool hangsOnCliff = false;
public:
  Character(Level& level, float x, float y) {
    level_ = &level;
    texture.loadFromFile("player.png");

    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    BodyDef.type = b2_dynamicBody;
    BodyDef.fixedRotation = true;
    Body = level.World.CreateBody(&BodyDef);

    level.World.SetContactListener(&level.hasContact);

    b2CircleShape Shape;
    Shape.m_p.Set(0, 0);
    Shape.m_radius = (14) / SCALE;

    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.0f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
    Body->SetUserData((void*)this);
  }

  enum class MoveDirection {
    LEFT,
    RIGHT,
    NONE
  };
  MoveDirection MoveDir = MoveDirection::NONE;

  Tile& getTile(int dx = 0, int dy = 0) {
    int tx = (int) (Body->GetPosition().x + 0.5f) + dx;
    int ty = (int) (Body->GetPosition().y + 0.5f) + dy;
    return level_->get(tx, ty);
  }

  virtual void update() override {
    hangsOnCliff = false;
    switch(MoveDir) {
      case MoveDirection::LEFT:
        Body->SetLinearVelocity({-3, Body->GetLinearVelocity().y});
        looksRight = false;
        if (getTile(-1).hasCliff() && hasContactInDirection(3.14, b2_pi)) {
          if ((int) (Body->GetPosition().y * 2) % 2 == 0) {
            if (Body->GetLinearVelocity().y >= 0) {
              Body->SetLinearVelocity({Body->GetLinearVelocity().x, 0});
              Body->ApplyForceToCenter({0, -GRAVITY}, true);
            }
          }
          hangsOnCliff = true;
        }
        break;
      case MoveDirection::RIGHT:
        Body->SetLinearVelocity({3, Body->GetLinearVelocity().y});
        looksRight = true;
        if (getTile(1).hasCliff() && hasContactInDirection(0, b2_pi)) {
          if ((int) (Body->GetPosition().y * 2) % 2 == 0) {
            if (Body->GetLinearVelocity().y >= 0) {
              Body->SetLinearVelocity({Body->GetLinearVelocity().x, 0});
              Body->ApplyForceToCenter({0, -GRAVITY}, true);
            }
          }
          hangsOnCliff = true;
        }
        break;
      case MoveDirection::NONE:
        Body->SetLinearVelocity({0, Body->GetLinearVelocity().y});
        break;
    }
  }

  virtual void render(sf::RenderTarget& target) override {
    sf::Sprite sprite(texture);
    if (looksRight)
      sprite.scale(-1, 1);

    sprite.setOrigin(11.f, 14.f);
    sprite.setPosition(SCALE * Body->GetPosition().x,
                       SCALE * Body->GetPosition().y);
    sprite.setRotation(Body->GetAngle() * 180 / b2_pi);
    target.draw(sprite);
  }

  bool hasContactInDirection(double direction, double precision) {
    for (b2ContactEdge* edge = Body->GetContactList(); edge; edge = edge->next) {
      auto contact = edge->contact;
      if (!contact->IsTouching())
        continue;

      auto A = (GameObject*) contact->GetFixtureA()->GetBody()->GetUserData();
      auto B = (GameObject*) contact->GetFixtureB()->GetBody()->GetUserData();
      b2WorldManifold worldManifold;
      contact->GetWorldManifold(&worldManifold);
      double d;
      if (auto Char = dynamic_cast<Character *>(A)) {
        d = std::atan2(worldManifold.normal.y, worldManifold.normal.x);
      } else {
        d = std::atan2(worldManifold.normal.y, worldManifold.normal.x);
      }
      if (std::abs(d - direction) < precision)
        return true;
      if (std::abs(d - direction) - b2_pi < precision)
        return true;
    }
    return false;
  }

  void jump() {

    bool hasFloorContact = false;
    if (!hangsOnCliff) {
      if (Body->GetLinearVelocity().y < 0.02) {
        hasFloorContact = hasContactInDirection(-b2_pi/2, b2_pi * 0.9f);
      }
    }

    if (hangsOnCliff || hasFloorContact) {
      Body->SetLinearVelocity({Body->GetLinearVelocity().x, -7});
    }
  }
};

class Snake : public GameObject {

  bool looksRight = true;
  b2Body* Body;
  sf::Texture texture;
  Level* level_;

public:
  Snake(Level& level, float x, float y) {
    level_ = &level;
    texture.loadFromFile("snake.png");

    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x / SCALE, y / SCALE);
    BodyDef.type = b2_dynamicBody;
    BodyDef.fixedRotation = true;
    Body = level.World.CreateBody(&BodyDef);

    b2CircleShape Shape;
    Shape.m_p.Set(0, 0);
    Shape.m_radius = 7 / SCALE;

    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.0f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
    Body->SetUserData((void*)this);
  }

  enum class MoveDirection {
    LEFT,
    RIGHT,
    NONE
  };
  MoveDirection MoveDir = MoveDirection::RIGHT;

  Tile& getTile(int dx = 0, int dy = 0) {
    int tx = (int) (Body->GetPosition().x + 0.5f) + dx;
    int ty = (int) (Body->GetPosition().y + 0.5f) + dy;
    return level_->get(tx, ty);
  }

  virtual void update() override {
    if (std::abs(Body->GetLinearVelocity().x) < 0.1) {
      if (MoveDir == MoveDirection::LEFT)
        MoveDir = MoveDirection::RIGHT;
      else
        MoveDir = MoveDirection::LEFT;
    }
    switch(MoveDir) {
      case MoveDirection::LEFT:
        Body->SetLinearVelocity({-1, Body->GetLinearVelocity().y});
        looksRight = false;
        break;
      case MoveDirection::RIGHT:
        Body->SetLinearVelocity({1, Body->GetLinearVelocity().y});
        looksRight = true;
        break;
      case MoveDirection::NONE:
        Body->SetLinearVelocity({0, Body->GetLinearVelocity().y});
        break;
    }
  }

  virtual void render(sf::RenderTarget& target) override {
    sf::Sprite sprite(texture);
    int spriteIndex = 0;
    if ((int) (level_->time * 2) % 2)
      spriteIndex = 1;
    sprite.setTextureRect({18 * spriteIndex, 0, 18, 14});
    if (!looksRight)
      sprite.scale(-1, 1);

    sprite.setOrigin(9.f, 7.f);
    sprite.setPosition(SCALE * Body->GetPosition().x,
                       SCALE * Body->GetPosition().y);
    sprite.setRotation(Body->GetAngle() * 180 / b2_pi);
    target.draw(sprite);
  }

};

class Stone : public GameObject {

  b2Body* Body;

  sf::Texture texture;

public:
  Stone(Level& level, float x, float y) {
    texture.loadFromFile("box.png");

    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(x, y);
    BodyDef.type = b2_dynamicBody;
    BodyDef.fixedRotation = true;
    Body = level.World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 5.f;
    FixtureDef.friction = 0.9f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
    Body->SetUserData((void*)this);
  }

  virtual void update() override {}

  virtual void render(sf::RenderTarget& target) override {
    sf::Sprite sprite(texture);

    sprite.setOrigin(16.f, 16.f);
    sprite.setPosition(SCALE * Body->GetPosition().x,
                       SCALE * Body->GetPosition().y);
    sprite.setRotation(Body->GetAngle() * 180 / b2_pi);
    target.draw(sprite);
  }

};

int main() {
  srand(time(NULL));

  /** Prepare the window */
  sf::RenderWindow Window(sf::VideoMode(800, 600, 32), "Test");
  Window.setFramerateLimit(60);

  GameData Data(".");

  Level level(40, 40, Data);

  /** Prepare textures */
  sf::Texture GroundTexture;
  sf::Texture BoxTexture;
  GroundTexture.loadFromFile("stone.png");
  BoxTexture.loadFromFile("box.png");

  Character* player = new Character(level, 40, 40);
  level.add(player);

  for (int i = 0; i < 10; ++i)
    level.add(new Snake(level, rand() % 1000, 0));

  while (Window.isOpen()) {
    sf::Event event;
    while (Window.pollEvent(event)) {
      if (event.type == sf::Event::EventType::Closed) {
        return 0;
      }
      if (event.type == sf::Event::EventType::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space)
          player->jump();
        if (event.key.code == sf::Keyboard::Escape)
          return 0;
      }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)
        == sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      player->MoveDir = Character::MoveDirection::NONE;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      player->MoveDir = Character::MoveDirection::RIGHT;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      player->MoveDir = Character::MoveDirection::LEFT;
    }
    level.update();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      int MouseX = sf::Mouse::getPosition(Window).x;
      int MouseY = sf::Mouse::getPosition(Window).y;
      Stone* stone = new Stone(level, MouseX / SCALE, MouseY / SCALE);
      level.add(stone);
    }
    level.World.Step(1 / 60.f, 8, 3);

    Window.clear(sf::Color::Black);

    level.render(Window);
    Window.display();
  }

  return 0;
}
