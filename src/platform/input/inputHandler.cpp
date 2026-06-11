#include "inputHandler.hpp"
#include "engine/eventManager/eventManager.hpp"
#include "utils/logger/logger.hpp"
#include <GLFW/glfw3.h>

InputHandler::InputHandler(GLFWwindow *window) : window(window) {
  glfwSetScrollCallback(window, InputHandler::wheelCallback);
  glfwSetKeyCallback(window,[](GLFWwindow* , int key, int , int action, int ){
      if (action == GLFW_PRESS) EventManager::emit(KeyboardKeyPressedEvent{static_cast<Key>(key)});
  });
  glfwSetMouseButtonCallback(window,[](GLFWwindow* , int button, int action, int ){
      if (action == GLFW_PRESS) EventManager::emit(MouseButtonPressedEvent{static_cast<Mouse>(button)});
      });
}

bool InputHandler::checkKeyPress(Key key) {
  int glfwKey = static_cast<int>(key);
  if(glfwGetKey(window, glfwKey) == GLFW_PRESS){
    LOG_DEBUG("key pressed: {}",keyToString(key));
    return true;
  };
  return false;
}


bool InputHandler::checkMousePress(Mouse button){
  int state = glfwGetMouseButton(window, (int)button);
  if (state == GLFW_PRESS){
    LOG_DEBUG("mouse key pressed : {}",(int)button);
    return true;
  }
  return false;
}



vec2 InputHandler::getCursorPos(){
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  return {xpos,ypos};
}



const char* keyToString(Key key) {
    switch (key) {
        case Key::Space: return "Space";
        case Key::Apostrophe: return "Apostrophe";
        case Key::Comma: return "Comma";
        case Key::Minus: return "Minus";
        case Key::Period: return "Period";
        case Key::Slash: return "Slash";
        
        case Key::_0: return "0";
        case Key::_1: return "1";
        case Key::_2: return "2";
        case Key::_3: return "3";
        case Key::_4: return "4";
        case Key::_5: return "5";
        case Key::_6: return "6";
        case Key::_7: return "7";
        case Key::_8: return "8";
        case Key::_9: return "9";
        
        case Key::A: return "A";
        case Key::B: return "B";
        case Key::C: return "C";
        case Key::D: return "D";
        case Key::E: return "E";
        case Key::F: return "F";
        case Key::G: return "G";
        case Key::H: return "H";
        case Key::I: return "I";
        case Key::J: return "J";
        case Key::K: return "K";
        case Key::L: return "L";
        case Key::M: return "M";
        case Key::N: return "N";
        case Key::O: return "O";
        case Key::P: return "P";
        case Key::Q: return "Q";
        case Key::R: return "R";
        case Key::S: return "S";
        case Key::T: return "T";
        case Key::U: return "U";
        case Key::V: return "V";
        case Key::W: return "W";
        case Key::X: return "X";
        case Key::Y: return "Y";
        case Key::Z: return "Z";
        
        case Key::LeftBracket: return "LeftBracket";
        case Key::Backslash: return "Backslash";
        case Key::RightBracket: return "RightBracket";
        case Key::GraveAccent: return "GraveAccent";
        
        case Key::Escape: return "Escape";
        case Key::Enter: return "Enter";
        case Key::Tab: return "Tab";
        case Key::Backspace: return "Backspace";
        case Key::Insert: return "Insert";
        case Key::Delete: return "Delete";
        case Key::Right: return "Right";
        case Key::Left: return "Left";
        case Key::Down: return "Down";
        case Key::Up: return "Up";
        case Key::PageUp: return "PageUp";
        case Key::PageDown: return "PageDown";
        case Key::Home: return "Home";
        case Key::End: return "End";
        case Key::CapsLock: return "CapsLock";
        case Key::ScrollLock: return "ScrollLock";
        case Key::NumLock: return "NumLock";
        case Key::PrintScreen: return "PrintScreen";
        case Key::Pause: return "Pause";
        
        case Key::F1: return "F1";
        case Key::F2: return "F2";
        case Key::F3: return "F3";
        case Key::F4: return "F4";
        case Key::F5: return "F5";
        case Key::F6: return "F6";
        case Key::F7: return "F7";
        case Key::F8: return "F8";
        case Key::F9: return "F9";
        case Key::F10: return "F10";
        case Key::F11: return "F11";
        case Key::F12: return "F12";
        case Key::F13: return "F13";
        case Key::F14: return "F14";
        case Key::F15: return "F15";
        case Key::F16: return "F16";
        case Key::F17: return "F17";
        case Key::F18: return "F18";
        case Key::F19: return "F19";
        case Key::F20: return "F20";
        case Key::F21: return "F21";
        case Key::F22: return "F22";
        case Key::F23: return "F23";
        case Key::F24: return "F24";
        case Key::F25: return "F25";
        
        case Key::KP_0: return "KP_0";
        case Key::KP_1: return "KP_1";
        case Key::KP_2: return "KP_2";
        case Key::KP_3: return "KP_3";
        case Key::KP_4: return "KP_4";
        case Key::KP_5: return "KP_5";
        case Key::KP_6: return "KP_6";
        case Key::KP_7: return "KP_7";
        case Key::KP_8: return "KP_8";
        case Key::KP_9: return "KP_9";
        case Key::KP_Decimal: return "KP_Decimal";
        case Key::KP_Divide: return "KP_Divide";
        case Key::KP_Multiply: return "KP_Multiply";
        case Key::KP_Subtract: return "KP_Subtract";
        case Key::KP_Add: return "KP_Add";
        case Key::KP_Enter: return "KP_Enter";
        case Key::KP_Equal: return "KP_Equal";
        
        case Key::LeftShift: return "LeftShift";
        case Key::LeftControl: return "LeftControl";
        case Key::LeftAlt: return "LeftAlt";
        case Key::LeftSuper: return "LeftSuper";
        case Key::RightShift: return "RightShift";
        case Key::RightControl: return "RightControl";
        case Key::RightAlt: return "RightAlt";
        case Key::RightSuper: return "RightSuper";
        
        case Key::Menu: return "Menu";
        
        default: return "Unknown";
    }
}
