#include "Window.h"

namespace util
{
	Window::Window(int size)
	{
		this->size = size;
		this->windowSum = 0.0f;

		// Go ahead and fill it, assuming it will be the same size for this window's lifetime (and calculate sum in case)
		this->window.realloc(size);
		for (int i = 0; i < size; i++)
		{
			this->window[i] = hann(i);
			windowSum += this->window[i];
		}
	}

	float Window::GetWeight(int index)
	{
		return this->window[index];
	}

	float Window::GetSum()
	{
		return this->windowSum;
	}

	float Window::GetScaleFactor()
	{
		return 1.0f / 4.0f / this->windowSum * this->size;
	}

	float Window::hann(float sample)
	{
		return 0.5f * (1.0f - cosf(2.0 * M_PI * sample / (float)(this->size - 1)));
	}
}