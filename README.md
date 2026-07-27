# Lil'OS
A micro OS built from the ground up in C++ for embedded systems

In its current state we only have a few simple shape renderers and a text renderer that both draw to a framebuffer to be drawn. The framebuffer is pushed to the screen. So far an SDL HAL has been implemented to allow testing on pc.

### Prerequisites
You must install SDL2 to run the program.
The python [font-to-bitmap tool](tools/fontgen.py) requires PIL and TTFont.

## 🛠️ Features
- [x] Basic shape renderer
- [x] Text char and string renderer
- [x] SDL2 hardware abstraction layer
- [ ] Dynamic rendering (using input to move and draw things)
- [ ] A mini console for inputting our own commands