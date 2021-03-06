#pragma once

#include <EOUL/String.hpp>

#include <string>
#include <sstream>
#include <iostream>

namespace EOUL::Math {

	class Color final {

		friend std::ostream& operator<<(std::ostream& os, const Color& color);

		private:

			union Storage {

				unsigned int decimal = 0;

				struct {

					unsigned char a;
					unsigned char b;
					unsigned char g;
					unsigned char r;

				};

			} storage;

			unsigned char& r = this->storage.r;
			unsigned char& g = this->storage.g;
			unsigned char& b = this->storage.b;
			unsigned char& a = this->storage.a;
			unsigned int& decimal = this->storage.decimal;

		public:

			Color() = default;

			inline Color(unsigned int decimal) {

				this->decimal = decimal;

			}

			inline Color(std::string hex) {

				if (Util::startsWith(hex, "0x")) {

					hex.erase(0, 2);

				} else if (Util::startsWith(hex, "#")) {

					hex.erase(0, 1);

				}

				this->decimal = (unsigned int) std::stoul(hex, nullptr, 16);

			}

			inline Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {

				this->r = r;
				this->g = g;
				this->b = b;
				this->a = a;

			}

			inline Color(Color&& other) noexcept {

				this->swap(other);

			}

		public:

			/* swaps 2 colors */
			inline void swap(Color& other) noexcept {

				std::swap(this->decimal, other.decimal);

			}

			/* returns an integer representation of the rgba values */
			inline unsigned int getRGBA() const {

				return this->decimal;

			}

			/* returns the red channel with a scale from 0 - 255 */
			inline unsigned char getRed() const {

				return this->r;

			}
			/* returns the green channel with a scale from 0 - 255 */
			inline unsigned char getGreen() const {

				return this->g;

			}
			/* returns the blue channel with a scale from 0 - 255 */
			inline unsigned char getBlue() const {

				return this->b;

			}
			/* returns the alpha channel with a scale from 0 - 255 */
			inline unsigned char getAlpha() const {

				return this->a;

			}

			/* returns the hex representation of the color */
			inline std::string getHex() const {

				return Color::decimalToHex(this->getRGBA());

			}

			/* sets red, green, blue and alpha values with an unsigned int */
			inline void setRGBA(unsigned int rgba) {

				this->decimal = rgba;

			}
			/* sets the red value */
			inline void setRed(unsigned char red) {

				this->r = red;

			}
			/* sets the green value */
			inline void setGreen(unsigned char green) {

				this->g = green;

			}
			/* sets the blue value */
			inline void setBlue(unsigned char blue) {

				this->b = blue;

			}
			/* sets the alpha value */
			inline void setAlpha(unsigned char alpha) {

				this->a = alpha;

			}

		public:

			inline bool operator==(const Color& other) const {

				return this->decimal == other.storage.decimal;

			}

			inline bool operator!=(const Color& other) const {

				return this->decimal != other.storage.decimal;

			}

			inline Color& operator=(const Color& other) {

				this->decimal = other.storage.decimal;

				return *this;

			}

			inline Color& operator+=(const Color& other) {

				unsigned int prev = this->decimal;

				this->decimal += other.storage.decimal;

				return *this;

			}

			inline Color& operator-=(const Color& other) {

				unsigned int prev = this->decimal;

				this->decimal -= other.storage.decimal;
		
				return *this;

			}

			inline unsigned char operator[](unsigned int index) const {

				switch (index) {

					case 0:

						return this->r;

					case 1:

						return this->g;

					case 2:

						return this->b;

					case 3:

						return this->a;

					default:

						throw std::exception("Index out of bounds!");

				}

			}

		public:

			/* returns a hex representation of the given color */
			inline static std::string decimalToHex(unsigned int color) {

				std::stringstream ss;

				ss << std::hex << color;

				return ss.str();

			}
			/* returns a decimal representation of the given hex string */
			inline static unsigned int hexToDecimal(std::string hex) {

				if (Util::startsWith(hex, "0x")) {

					hex.erase(0, 2);

				} else if (Util::startsWith(hex, "#")) {

					hex.erase(0, 1);

				}

				return (unsigned int) std::stoul(hex, nullptr, 16);

			}

	};

	inline std::ostream& operator<<(std::ostream& os, const Color& color) {

		return os << "Color(r: " << (int) color.r << ", g: " << (int) color.g << ", b: " << (int) color.b << ", a: " << (int) color.a << ")";

	}

	inline Color operator+(Color left, const Color& other) {

		left += other;

		return left;

	}

	inline Color operator-(Color left, const Color& other) {

		left -= other;

		return left;

	}

}