#pragma once

#include <string>
#include <iostream>
#undef MOUSE_MOVED

namespace Lighthouse
{
	enum class EventType
	{
		NONE = 0,
		WINDOW_CLOSE, WINDOW_RESIZE,
		KEY_PRESSED, KEY_RELEASED, KEY_TYPED,
		MOUSE_MOVED, MOUSE_SCROLLED, MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED 
	};

	class Event
	{
	public:
		virtual ~Event() = default;

		bool handled = false;

		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual std::string toString() const { return getName(); }
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event) : _event(event) {}

		template<typename T, typename F>
		bool dispatch(const F& func)
		{
			if (_event.getEventType() == T::getStaticType())
			{
				_event.handled |= func(static_cast<T&>(_event));
				return true;
			}
			return false;
		}

	private:
		Event& _event;
	};

}
