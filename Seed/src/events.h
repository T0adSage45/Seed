#pragma once

#include <pch.h>
#include "core.h"

namespace Seed {

enum class EventType {
    NONE = 0,
    KEY_PRESSED,
    KEY_RELEASED,
    KEY_TYPED,
    MOUSE_BUTTON_PRESSED,
    MOUSE_BUTTON_RELEASED,
    MOUSE_MOVED,
    MOUSE_SCROLLED,
    WINDOW_CLOSED,
    WINDOW_RESIZED,
    WINDOW_MOVED,
    WINDOW_FOCUS,
    WINDOW_DISTRACT
};

enum EventCategory {
    NONE = 0,
    KEYBOARD_EVENTS = BIT(0),
    INPUT_EVENT = BIT(1),
    WINDOW_EVENT = BIT(2),
    MOUSE_EVENT = BIT(3)
};

// macros
#define EVENT_CLASS_TYPE(type)                                                  \
    static EventType GetStaticType() { return EventType::type; }                \
    virtual EventType GetEventType() const override { return GetStaticType(); } \
    virtual const char *GetEventName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    virtual int GetCategoryFlags() const override { return category; }

// events
class SEED_API Event {
public:
    virtual ~Event() = default;
    virtual EventType GetEventType() const = 0;
    virtual const char *GetEventName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetEventName(); }

    inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }

    inline bool IsHandled() const { return m_Handled; }
    inline void SetHandled(bool handled) { m_Handled = handled; }

protected:
    bool m_Handled = false;

private:
    friend class EventDispatcher;
};

/// evetnt dispatcher
class SEED_API EventDispatcher {
    template <typename T>
    using EventFn = std::function<bool(T &)>;

public:
    EventDispatcher(Event &e)
        : m_Event(e) {}

    template <typename T>
    bool Dispatch(EventFn<T> func) {
        if (m_Event.GetEventType() == T::GetStaticType() && !m_Event.IsHandled()) {
            m_Event.SetHandled(func(static_cast<T &>(m_Event)));
            return true;
        }
        return false;
    }

private:
    Event &m_Event;
};

// keyboard events
class SEED_API KeyEvent : public Event {
public:
    inline int GetKeyCode() const { return m_KeyCode; }
    EVENT_CLASS_CATEGORY(KEYBOARD_EVENTS | INPUT_EVENT);

protected:
    KeyEvent(int keycode)
        : m_KeyCode(keycode) {}
    int m_KeyCode;
};

class SEED_API KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(int keycode, int repeatCount)
        : KeyEvent(keycode),
          m_RepeatCount(repeatCount) {}
    inline int GetRepeatCount() const { return m_RepeatCount; }
    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
        return ss.str();
    }
    EVENT_CLASS_TYPE(KEY_PRESSED);

private:
    int m_RepeatCount;
};

class SEED_API KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(int keycode)
        : KeyEvent(keycode) {}
    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << m_KeyCode;
        return ss.str();
    }
    EVENT_CLASS_TYPE(KEY_RELEASED);
};

class SEED_API KeyTypedEvent : public KeyEvent {
public:
    KeyTypedEvent(int keycode)
        : KeyEvent(keycode) {}
    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyTypedEvent: " << m_KeyCode;
        return ss.str();
    }
    EVENT_CLASS_TYPE(KEY_TYPED);
};

// mouse events
class SEED_API MouseEvent : public Event {
public:
    inline float GetX() const { return m_X; }
    inline float GetY() const { return m_Y; }
    EVENT_CLASS_CATEGORY(MOUSE_EVENT | INPUT_EVENT);

protected:
    MouseEvent(float x, float y)
        : m_X(x),
          m_Y(y) {}
    float m_X, m_Y;
};

class SEED_API MouseButtonPressedEvent : public MouseEvent {
public:
    MouseButtonPressedEvent(float x, float y, int button)
        : MouseEvent(x, y),
          m_Button(button) {}
    inline int GetButton() const { return m_Button; }
    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonPressed: " << m_Button << " at (" << m_X << ", " << m_Y << ")";
        return ss.str();
    }
    EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESSED);

private:
    int m_Button;
};

class SEED_API MouseButtonReleasedEvent : public MouseEvent {
public:
    MouseButtonReleasedEvent(float x, float y, int button)
        : MouseEvent(x, y),
          m_Button(button) {}
    inline int GetButton() const { return m_Button; }
    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonReleased: " << m_Button << " at (" << m_X << ", " << m_Y << ")";
        return ss.str();
    }
    EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASED);

private:
    int m_Button;
};

class SEED_API MouseMovedEvent : public MouseEvent {
public:
    MouseMovedEvent(float x, float y, float dx, float dy)
        : MouseEvent(x, y),
          m_DeltaX(dx),
          m_DeltaY(dy) {}
    inline float GetDeltaX() const { return m_DeltaX; }
    inline float GetDeltaY() const { return m_DeltaY; }
    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseMoved: (" << m_X << ", " << m_Y << ") delta (" << m_DeltaX << ", " << m_DeltaY
           << ")";
        return ss.str();
    }
    EVENT_CLASS_TYPE(MOUSE_MOVED);

private:
    float m_DeltaX, m_DeltaY;
};

class SEED_API MouseScrolledEvent : public MouseEvent {
public:
    MouseScrolledEvent(float x, float y, float scrollX, float scrollY)
        : MouseEvent(x, y),
          m_ScrollX(scrollX),
          m_ScrollY(scrollY) {}
    inline float GetScrollX() const { return m_ScrollX; }
    inline float GetScrollY() const { return m_ScrollY; }
    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseScrolled: (" << m_X << ", " << m_Y << ") scroll (" << m_ScrollX << ", "
           << m_ScrollY << ")";
        return ss.str();
    }
    EVENT_CLASS_TYPE(MOUSE_SCROLLED);

private:
    float m_ScrollX, m_ScrollY;
};

// window events
class SEED_API WindowEvent : public Event {
public:
    EVENT_CLASS_CATEGORY(WINDOW_EVENT);
};

class SEED_API WindowClosedEvent : public WindowEvent {
public:
    WindowClosedEvent() = default;
    EVENT_CLASS_TYPE(WINDOW_CLOSED);
};

class SEED_API WindowResizedEvent : public WindowEvent {
public:
    WindowResizedEvent(unsigned int width, unsigned int height)
        : m_Width(width),
          m_Height(height) {}
    inline unsigned int GetWidth() const { return m_Width; }
    inline unsigned int GetHeight() const { return m_Height; }
    std::string ToString() const override {
        std::stringstream ss;
        ss << "WindowResizedEvent: " << m_Width << ", " << m_Height;
        return ss.str();
    }
    EVENT_CLASS_TYPE(WINDOW_RESIZED);

private:
    unsigned int m_Width, m_Height;
};

class SEED_API WindowMovedEvent : public WindowEvent {
public:
    WindowMovedEvent(int x, int y)
        : m_X(x),
          m_Y(y) {}
    inline int GetX() const { return m_X; }
    inline int GetY() const { return m_Y; }
    std::string ToString() const override {
        std::stringstream ss;
        ss << "WindowMovedEvent: " << m_X << ", " << m_Y;
        return ss.str();
    }
    EVENT_CLASS_TYPE(WINDOW_MOVED);

private:
    int m_X, m_Y;
};

class SEED_API WindowFocusEvent : public WindowEvent {
public:
    WindowFocusEvent() = default;
    std::string ToString() const override { return "WindowFocusEvent"; }
    EVENT_CLASS_TYPE(WINDOW_FOCUS);
};

class SEED_API WindowLostFocusEvent : public WindowEvent {
public:
    WindowLostFocusEvent() = default;
    std::string ToString() const override { return "WindowLostFocusEvent"; }
    EVENT_CLASS_TYPE(WINDOW_DISTRACT);
};

} // namespace Seed
