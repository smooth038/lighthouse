#pragma once

#include "Event.h"
#include <sstream>

namespace Lighthouse 
{

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		static EventType getStaticType()
		{
			return EventType::WINDOW_CLOSE;
		}

		EventType getEventType() const override
		{
			return getStaticType();
		}

		const char* getName() const override
		{
			return "WindowCloseEvent";
		}
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) : _width(width), _height(height) {}

		static EventType getStaticType() 
		{
			return EventType::WINDOW_RESIZE;
		}

		EventType getEventType() const override
		{
			return getStaticType();
		}

		const char* getName() const override
		{
			return "WindowResizeEvent";
		}

		unsigned int getWidth() const { return _width; }
		unsigned int getHeight() const { return _height; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << _width << ", " << _height;
			return ss.str();
		}

	private:
		unsigned int _width, _height;
	};


}
