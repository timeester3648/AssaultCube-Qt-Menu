#pragma once

#include <EOUL\File.h>
#include <string>

namespace EOUL {

	namespace IO {

		class Image : public FileBase {

			private:

				unsigned char* pixels = nullptr;
				unsigned int width, height, numComponents;

			public:

				Image() = default;
				Image(std::string path);
				~Image();

			public:

				/* loads/reloads image and deletes current pixel data */
				void load();

				/* returns the red channel with a scale from 0 - 255, of the specified pixel */
				unsigned char getRed(int x, int y) const;
				/* returns the green channel with a scale from 0 - 255, of the specified pixel */
				unsigned char getGreen(int x, int y) const;
				/* returns the blue channel with a scale from 0 - 255, of the specified pixel */
				unsigned char getBlue(int x, int y) const;
				/* returns the alpha channel with a scale from 0 - 255, of the specified pixel */
				unsigned char getAlpha(int x, int y) const;

				/* returns width of the image */
				unsigned int getWidth();
				/* returns height of the image */
				unsigned int getHeight();

				/* returns an unsigned integer representation of the rgba values, of the specified pixel */
				unsigned int getRGBA(int x, int y) const;
				/* returns a pointer to the pixel array */
				unsigned char* getPixels() const;

		};

	}

}