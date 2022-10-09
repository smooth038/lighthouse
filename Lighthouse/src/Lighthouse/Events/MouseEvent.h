#pragma once

#include "lhpch.h"
#include "Event.h"
#include "MouseCode.h"
#undef MOUSE_MOVED

namespace Lighthouse
{

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y) : _mouseX(x), _mouseY(y) {}
		float getX() { return _mouseX; };
		float getY() { return _mouseY; };

		static EventType getStaticType() { return EventType::MOUSE_MOVED;  }
		virtual EventType getEventType() const override{ return getStaticType(); };
		virtual const char* getName() const override{ return "MouseMoved"; }

	private:
		float _mouseX, _mouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float offsetX, const float offsetY) : _offsetX(offsetX), _offsetY(offsetY) {}
		float getOffsetX() { return _offsetX; };
		float getOffsetY() { return _offsetY; };

		static EventType getStaticType() { return EventType::MOUSE_SCROLLED;  }
		virtual EventType getEventType() const override{ return getStaticType(); };
		virtual const char* getName() const override{ return "MouseScrolled"; }

	private:
		float _offsetX, _offsetY;
	};

	class MouseButtonEvent : public Event
	{
	public:
		float getMouseButton() { return _button; };

	protected:
		MouseButtonEvent(const MouseCode button) : _button(button) {}
		MouseCode _button;

	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const MouseCode button) : MouseButtonEvent(button) {}

		static EventType getStaticType() { return EventType::MOUSE_BUTTON_PRESSED;  }
		virtual EventType getEventType() const override{ return getStaticType(); };
		virtual const char* getName() const override{ return "MouseButtonPressed"; }
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const MouseCode button) : MouseButtonEvent(button) {}

		static EventType getStaticType() { return EventType::MOUSE_BUTTON_RELEASED;  }
		virtual EventType getEventType() const override{ return getStaticType(); };
		virtual const char* getName() const override{ return "MouseButtonReleased"; }
	};
}
