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
            /**
             * @brief Launch the music
             * 
             * @return int 
             */
                int playMusic() {
                    if (!this->_music->openFromFile(_musicPath))
                        return 84;
                    this->_music->play();
                    return 0;
                }
            /**
             * @brief Pause the music
             * 
             * @return int 
             */
                int pauseMusic() {
                    if (this->_music->getStatus() != sf::Music::Playing)
                        return 84;
                    this->_music->pause();
                    return 0;
                }
            /**
             * @brief Stop the music
             * 
             * @return int 
             */
                int stopmusic() {
                    if (this->_music->getStatus() != sf::Music::Playing)
                        return 84;
                    this->_music->stop();
                    return 0;
                }
            /**
             * @brief Set the Loop object
             * 
             * @param loop 
             */
                void setLoop(bool loop) {
                    this->_music->setLoop(loop);
                }
            /**
             * @brief Set the Volume object
             * 
             * @param volume 
             */
                void setVolume(float volume) {
                    this->_music->setVolume(volume);
                }
            /**
             * @brief Get the Status object
             * 
             * @return sf::SoundSource::Status 
             */
                sf::SoundSource::Status getStatus() const {
                    return _music->getStatus();
                }
            /**
             * @brief Get the Volume object
             * 
             * @return float 
             */
                float getVolume() const {
                    return _music->getVolume();
                }
            private:
            /**
             * @brief Store the music path
             * 
             */
                std::string _musicPath;
            /**
             * @brief Variable of the music
             * 
             */
                std::shared_ptr<sf::Music> _music;
        };
    }
}
