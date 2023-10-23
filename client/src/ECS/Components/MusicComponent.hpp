#pragma once

#include "SFML/Audio.hpp"

namespace ECS {
    namespace components {
        class MusicComponent {
            public:
            /**
             * @brief Construct a new Music Component object
             *
             */
                MusicComponent(const std::string &musicPath, float volume = 100.0f, bool loop = false): _musicPath(musicPath) {
                    this->_music = std::make_shared<sf::Music>();
                    this->_music->openFromFile(_musicPath);
                    this->_music->setVolume(volume);
                    this->_music->setLoop(loop);
                }


                int playMusic() {
                    if (!this->_music->openFromFile(_musicPath))
                        return 84;
                    this->_music->play();
                    return 0;
                }

                int pauseMusic() {
                    if (this->_music->getStatus() != sf::Music::Playing)
                        return 84;
                    this->_music->pause();
                    return 0;
                }

                int stopmusic() {
                    if (this->_music->getStatus() != sf::Music::Playing)
                        return 84;
                    this->_music->stop();
                    return 0;
                }

                void setLoop(bool loop) {
                    this->_music->setLoop(loop);
                }

                void setVolume(float volume) {
                    this->_music->setVolume(volume);
                }

                sf::SoundSource::Status getStatus() const {
                    return _music->getStatus();
                }

                float getVolume() const {
                    return _music->getVolume();
                }

            private:
                std::string _musicPath;
                std::shared_ptr<sf::Music> _music;
        };
    }
}
