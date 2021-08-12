#pragma once
#include <opencv2/highgui.hpp>
#include <opencv2/core/mat.hpp>



namespace RFramework
{
	class RGB
	{
		inline bool _Compare(uint8_t srcChannel, uint8_t dstChannel, uint8_t tolerance)
		{
			return abs(srcChannel - dstChannel) < tolerance;
		}
	public:
		void Set(uint8_t R, uint8_t G, uint8_t B)
		{
			this->R = R;
			this->G = G;
			this->B = B;
		}
		
		uint8_t R;
		uint8_t G;
		uint8_t B;

		bool CompareTo(const RGB& rgb, const RGB& tolerance)
		{
			if (!_Compare(this->R, rgb.R, tolerance.R))
				return false;
			if (!_Compare(this->G, rgb.G, tolerance.G))
				return false;
			return _Compare(this->B, rgb.B, tolerance.B);
		}
	};
	class RPixelMatching
	{
		static RGB GetPixelByCoord(const cv::Mat& img,int x, int y , int channels)
		{
			RGB rgb;
			uint8_t* pPixel = (uint8_t*)img.data;
			rgb.B = pPixel[x * img.cols * channels + y * channels + 0];
			rgb.G = pPixel[x * img.cols * channels + y * channels + 1];
			rgb.R = pPixel[x * img.cols * channels + y * channels + 2];
			return rgb;
		}

		
		static bool TamplateWalker(const cv::Mat& templ, const cv::Mat& image, int imageI,int imageJ)
		{
			static RGB tolerance;
			tolerance.Set(20, 20, 20);
			
			const int imageChannels = image.channels();
			
			uint8_t* pPixel = (uint8_t*)templ.data;
			const int channels = templ.channels();
			RGB templPixel;

			for (int i = 0; i < templ.rows; i++)
			{
				for (int j = 0; j < templ.cols; j++)
				{
					templPixel.B = pPixel[i * templ.cols * channels + j * channels + 0];
					templPixel.G = pPixel[i * templ.cols * channels + j * channels + 1];
					templPixel.R = pPixel[i * templ.cols * channels + j * channels + 2];

					if (!templPixel.CompareTo(GetPixelByCoord(image,imageI + i, imageJ + j, imageChannels), tolerance))
						return false;
				}
			}
			return true;
		}
		
		static cv::Point ImageWalker(const cv::Mat& image, const cv::Mat& templ)
		{
			cv::Point result(-1,-1);
			uint8_t* pPixel = (uint8_t*)image.data;
			const int channels = image.channels();
			RGB imagePixel;

			for (int i = 0; i < image.rows; i++)
			{
				for (int j = 0; j < image.cols; j++)
				{
					int index = i * image.cols * channels + j * channels + 0;
					
					imagePixel.B = pPixel[i * image.cols * channels + j * channels + 0]; // B
					imagePixel.G = pPixel[i * image.cols * channels + j * channels + 1]; // G
					imagePixel.R = pPixel[i * image.cols * channels + j * channels + 2]; // R

					if(TamplateWalker(templ,image,i,j))
					{
						result.x = j;
						result.y = i;
						return result;
					}
				}
			}
			return result;
		}
	
	public:
		static void Match(cv::Mat templ, cv::Mat image)
		{
			int x = 0;
			auto res = ImageWalker(image, templ);

			int y = 0;
			
			//PixelProcessor(Image, [&templ](const RGB& imagePixel)
			//	{
			//		PixelProcessor(templ, [&templ,&imagePixel](const RGB& templPixel)
			//			{
			//				imagePixel
			//				return false;
			//			}
			//		);
			//	
			//		return false;
			//	});
			
		}
	};
}