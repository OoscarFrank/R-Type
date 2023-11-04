#pragma once

#include "SFML/Audio.hpp"

#pragma once

#include <memory>
#include <string>
#include "SFML/Audio.hpp"

namespace ECS {
    namespace components {
        class SoundComponent {
            public:
                /**
                 * @brief Construct a new Sound Component object
                 *
                 */
                SoundComponent(const std::string &soundPath, float volume = 100.0f, bool play = false): _soundPath(soundPath) {
                    _soundBuffer = std::make_shared<sf::SoundBuffer>();
                    _soundBuffer->loadFromFile(_soundPath);

                    _sound = std::make_shared<sf::Sound>();
                    _sound->setBuffer(*_soundBuffer);
                    _sound->setVolume(volume);
                    if (play)
                        _sound->play();
                }
            /**
             * @brief Play the sound
             * 
             * @return int 
             */
                int playSound() {
                    _sound->play();
                    return 0;
                }
            /**
             * @brief Pause the sound
             * 
             * @return int 
             */
                int pauseSound() {
                    if (_sound->getStatus() != sf::Sound::Playing)
                        return 84;
                    _sound->pause();
                    return 0;
                }
            /**
             * @brief Stop the sound
             * 
             * @return int 
             */
                int stopSound() {
                    if (_sound->getStatus() != sf::Sound::Playing)
                        return 84;
                    _sound->stop();
                    return 0;
                }
            /**
             * @brief Set the Volume object
             * 
             * @param volume 
             */
                void setVolume(float volume) {
                    _sound->setVolume(volume);
                }
            /**
             * @brief Get the Status object
             * 
             * @return sf::SoundSource::Status 
             */
                sf::SoundSource::Status getStatus() const {
                    return _sound->getStatus();
                }
            /**
             * @brief Get the Volume object
             * 
             * @return float 
             */
                float getVolume() const {
                    return _sound->getVolume();
                }
            private:
            /**
             * @brief Store the path of the sound
             * 
             */
                std::string _soundPath;
            /**
             * @brief Store the sound buffer
             * 
             */
                std::shared_ptr<sf::SoundBuffer> _soundBuffer;
            /**
             * @brief Store the sound
             * 
             */
                std::shared_ptr<sf::Sound> _sound;
        };
    }
}
