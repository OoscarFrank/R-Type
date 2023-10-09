#include "./Game.hpp"
#include "./ECS/Components/Components.hpp"
#include "./ECS/Systems/Systems.hpp"

using namespace game;
using entity_t = std::size_t;

Game::Game() :
    _manager(Loader()),
    _factory(Factory(ecs)),
    _net(Network()),
    _roomId(0),
    _playerId(0),
    _startTimeLeft(0),
    _started(0)
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    this->_screenSize = {mode.width, mode.height};

    if (mode.isValid()) {
        this->_window.create(mode, "R-TYPE", sf::Style::Fullscreen);
    } else {
        this->_window.create(sf::VideoMode(mode.width, mode.height), "R-TYPE");
    }


    this->_window.setFramerateLimit(120);
    this->_lastTime = NOW;
    this->_net.setInst(9);
    this->_net.send();
    this->eventMemory = 0;
    this->_gameOver = false;

    this->_manager.loadTexture(client::getAssetPath("parallax/background.png"), Loader::toLoad::ParallaxFirstbkg);
    this->_manager.loadTexture(client::getAssetPath("parallax/background2.png"), Loader::toLoad::ParallaxSecondbkg);

    int  divider = 1;
    #ifdef SFML_SYSTEM_MACOS
        divider = 2;
    #endif
    _resMult = (float)(this->_screenSize.x / divider)/ SCREEN_WIDTH;

    this->_factory.createParallax(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::ParallaxFirstbkg), -0.15f);
    this->_factory.createParallax(0.0f, 0.0f, this->_manager.getTexture(Loader::Loader::ParallaxSecondbkg), -0.20f);
}

Game::~Game()
{
}

entity_t Game::getPlayerEntityFromId(unsigned int id)
{
    for (auto &player : this->_players) {
        if (player.first == id)
            return player.second;
    }
    return 0;
}

entity_t Game::getMissileEntityFromId(unsigned int id)
{
    for (auto &missile : this->_missiles) {
        if (missile.first == id)
            return missile.second;
    }
    return 0;
}

entity_t Game::getEnnemiEntityFromId(unsigned int id)
{
    for (auto &Ennemi : this->_ennemies) {
        if (Ennemi.first == id)
            return Ennemi.second;
    }
    return 0;
}

void Game::update()
{
    Network::Packet packet;

    while (this->_net.getQueueIn().tryPop(packet)) {
        if (packet.getInstruction() == 10) {    //you join room
            this->_roomId = packet.getData().getDataUInt();
            this->_playerId = packet.getData().getDataUInt();

            this->_manager.loadTexture(client::getAssetPath("entity/player/player_move1.png"), Loader::toLoad::Player_move1);

            entity_t newEntity = this->_factory.createPlayer(-1000.0f, -1000.0f, this->_manager.getTexture(Loader::Loader::Player_move1));
            this->_players.push_back(std::make_pair(this->_playerId, newEntity));
            this->_playerEntity = newEntity;

            this->_manager.loadTexture(client::getAssetPath("entity/missile/missile.png"), Loader::toLoad::Missile);
            this->_manager.loadTexture(client::getAssetPath("entity/monsters/monster1.png"), Loader::toLoad::Monster1);
            this->_manager.loadTexture(client::getAssetPath("entity/player/player_move1.png"), Loader::toLoad::Player_move1);
            this->_manager.loadTexture(client::getAssetPath("entity/player/player_move2.png"), Loader::toLoad::Player_move2);
            this->_manager.loadTexture(client::getAssetPath("entity/player/player_move3.png"), Loader::toLoad::Player_move3);
            this->_manager.loadTexture(client::getAssetPath("entity/player/player_move4.png"), Loader::toLoad::Player_move4);

        }

        if (packet.getInstruction() == 13) {    //player join game
            unsigned int id = packet.getData().getDataUInt();
            const sf::Texture *texture = nullptr;

            switch (id) {
                case 1:
                    texture = &this->_manager.getTexture(Loader::Loader::Player_move1);
                    break;
                case 2:
                    texture = &this->_manager.getTexture(Loader::Loader::Player_move2);
                    break;
                case 3:
                    texture = &this->_manager.getTexture(Loader::Loader::Player_move3);
                    break;
                case 4:
                    texture = &this->_manager.getTexture(Loader::Loader::Player_move4);
                    break;
                default:
                    texture = &this->_manager.getTexture(Loader::Loader::Player_move1);
                    break;
            }

            if (texture != nullptr) {
                entity_t newEntity = this->_factory.createPlayer(500.0f, 500.0f, *texture);
                this->_players.push_back(std::make_pair(id, newEntity));
                this->_playerEntity = newEntity;
            }
        }

        if (packet.getInstruction() == 11) {    //timeout matchmaking
            this->_startTimeLeft = packet.getData().getDataUInt();
            this->_started = packet.getData().getDataUChar();
        }

        if (packet.getInstruction() == 3) {     //players position
            unsigned int id = packet.getData().getDataUInt();
            unsigned short x = packet.getData().getDataUShort();
            x *= _resMult;
            unsigned short y = packet.getData().getDataUShort();
            y *= _resMult;
            this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(this->getPlayerEntityFromId(id), x, y));
        }

        if (packet.getInstruction() == 6) {     //player score
            unsigned int score = packet.getData().getDataUInt();
            std::cout << "Score: " << score << std::endl;
        }

        if (packet.getInstruction() == 4) {     //missile position
            unsigned int id = packet.getData().getDataUInt();
            unsigned char type = packet.getData().getDataUChar();
            unsigned short x = packet.getData().getDataUShort();
            x *= _resMult;
            unsigned short y = packet.getData().getDataUShort();
            y *= _resMult;

            (void)type;

            entity_t res = getMissileEntityFromId(id);
            if (res == 0) {
                entity_t newEntity = this->_factory.createMissile(x, y, this->_manager.getTexture(Loader::Loader::Missile));
                this->_missiles.push_back(std::make_pair(id, newEntity));
            } else {
                this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(res, x, y));
            }
        }

        if (packet.getInstruction() == 7) {     //ennemi position
            unsigned int id = packet.getData().getDataUInt();
            unsigned short x = packet.getData().getDataUShort();
            x *= _resMult;
            unsigned short y = packet.getData().getDataUShort();
            y *= _resMult;

            entity_t res = getEnnemiEntityFromId(id);

            if (res == 0) {
                entity_t newEntity = this->_factory.createEnnemi(x, y, this->_manager.getTexture(Loader::Loader::Monster1)); // TO REPLACE
                this->_ennemies.push_back(std::make_pair(id, newEntity));
            } else {
                this->_entityPositions.push_back(ECS::systems::MovableSystem::EntityPos(res, x, y));
            }
        }

        if (packet.getInstruction() == 18) {    //player died
            unsigned int id = packet.getData().getDataUInt();

            entity_t res = getPlayerEntityFromId(id);

            if (res != 0) {
                this->ecs.kill_entity(res);

                this->_entityPositions.erase(std::remove_if(this->_entityPositions.begin(), this->_entityPositions.end(), [id](ECS::systems::MovableSystem::EntityPos const &pair) {
                    return pair.getEntity() == id;
                }), this->_entityPositions.end());

                this->_players.erase(std::remove_if(this->_players.begin(), this->_players.end(), [id](std::pair<unsigned int, entity_t> const &pair) {
                    return pair.first == id;
                }), this->_players.end());
            }
        }

        if (packet.getInstruction() == 14) {    //player disconneted
            unsigned int id = packet.getData().getDataUInt();

            entity_t res = getPlayerEntityFromId(id);

            if (res != 0) {
                this->ecs.kill_entity(res);

                this->_entityPositions.erase(std::remove_if(this->_entityPositions.begin(), this->_entityPositions.end(), [id](ECS::systems::MovableSystem::EntityPos const &pair) {
                    return pair.getEntity() == id;
                }), this->_entityPositions.end());

                this->_players.erase(std::remove_if(this->_players.begin(), this->_players.end(), [id](std::pair<unsigned int, entity_t> const &pair) {
                    return pair.first == id;
                }), this->_players.end());
            }
        }

        if (packet.getInstruction() == 15) {    //missile destroyed
            unsigned int id = packet.getData().getDataUInt();
            unsigned char type = packet.getData().getDataUChar();
            unsigned short x = packet.getData().getDataUShort();
            x *= _resMult;
            unsigned short y = packet.getData().getDataUShort();
            y *= _resMult;

            entity_t res = getMissileEntityFromId(id);

            if (res != 0) {
                this->ecs.kill_entity(res);

                this->_entityPositions.erase(std::remove_if(this->_entityPositions.begin(), this->_entityPositions.end(), [id](ECS::systems::MovableSystem::EntityPos const &pair) {
                    return pair.getEntity() == id;
                }), this->_entityPositions.end());

                this->_missiles.erase(std::remove_if(this->_missiles.begin(), this->_missiles.end(), [id](std::pair<unsigned int, entity_t> const &pair) {
                    return pair.first == id;
                }), this->_missiles.end());
            }
        }

        if (packet.getInstruction() == 16) {        //ennemi died
            unsigned int id = packet.getData().getDataUInt();

            entity_t res = getEnnemiEntityFromId(id);

            if (res != 0) {
                this->ecs.kill_entity(res);

                this->_entityPositions.erase(std::remove_if(this->_entityPositions.begin(), this->_entityPositions.end(), [id](ECS::systems::MovableSystem::EntityPos const &pair) {
                    return pair.getEntity() == id;
                }), this->_entityPositions.end());

                this->_ennemies.erase(std::remove_if(this->_ennemies.begin(), this->_ennemies.end(), [id](std::pair<unsigned int, entity_t> const &pair) {
                    return pair.first == id;
                }), this->_ennemies.end());
            }
        }

        if (packet.getInstruction() == 17) {    //game over
            unsigned char type = packet.getData().getDataUChar();
            _gameOver = true;
        }
    }
    this->_net.setInst(12);
    this->_net.send();
}

void Game::sendMoveToServer()
{
    for (auto i = this->_entityEvents.begin(); i != this->_entityEvents.end(); ++i) {
        if (!_gameOver && (*i).getEntity() == this->_playerEntity) {
            char move = (*i).getEvent() & (UP | DOWN | LEFT | RIGHT);
            if ((*i).getEvent() & move) {
                this->_net.setInst(2);
                this->_net.getStreamOut().setDataUChar((*i).getEvent() & move);
                this->_net.getStreamOut().setDataUChar(1);
                this->_net.send();
            }
            if ((*i).getEvent() & SPACE) {
                this->_net.setInst(5);
                this->_net.send();
            }
        }
        
    }
    this->_entityEvents.clear();
}

int Game::MainLoop()
{
    while (this->_window.isOpen()) {
        long currentTime = NOW;
        float deltaTime = (currentTime - _lastTime) / 1.0f;
        _lastTime = currentTime;
        this->update();
        this->EventLoop(this->_window, this->_net);
        // ALL SYSTEMS CALL HERE (update)
        ECS::systems::ControllableSystem().update(this->ecs, this->_entityEvents, this->_window, this->eventMemory);
        ECS::systems::PositionSystem().update(this->ecs);
        ECS::systems::AnimationSystem().update(this->ecs, deltaTime);
        ECS::systems::ParallaxSystem().update(this->ecs, deltaTime);
        ECS::systems::MovableSystem().update(this->ecs, this->_entityPositions);
        ECS::systems::ScaleSystem().update(this->ecs);
        this->_window.clear();
        // DRAW SYSTEM CALL HERE (update) (after clear) (before display) (no update) (no event) (no loop) (no system call) (no event loop)
        ECS::systems::DrawSystem().update(this->ecs, this->_window);
        this->_window.display();
        this->sendMoveToServer();
    }
    this->_net.setClosed(true);
    return 0;
}
