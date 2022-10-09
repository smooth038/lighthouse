#pragma once

#include "lhpch.h"
#include "Event.h"
#include "KeyCode.h"

namespace Lighthouse
{

	class KeyEvent : public Event
	{
	public:
		KeyCode getKeyCode() { return _keyCode; };

	protected:
		KeyEvent(const KeyCode keycode) : _keyCode(keycode) {}
		KeyCode _keyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode keyCode, bool isRepeat = false) : KeyEvent(keyCode), _isRepeat(isRepeat) {}
		bool isRepeat() { return _isRepeat; }
		static EventType getStaticType() { return EventType::KEY_PRESSED;  }
		virtual EventType getEventType() const override{ return getStaticType(); };
		virtual const char* getName() const override{ return "KeyPressed"; }

	private:
		bool _isRepeat;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode keyCode) : KeyEvent(keyCode) {}
		static EventType getStaticType() { return EventType::KEY_RELEASED;  }
		virtual EventType getEventType() const override { return getStaticType(); };
		virtual const char* getName() const override{ return "KeyReleased"; }
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const KeyCode keyCode) : KeyEvent(keyCode) {}
		static EventType getStaticType() { return EventType::KEY_TYPED;  }
		virtual EventType getEventType() const override{ return getStaticType(); };
		virtual const char* getName() const override{ return "KeyTyped"; }
	};


}
