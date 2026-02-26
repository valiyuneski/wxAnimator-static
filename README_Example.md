# wxAnimator Example

This example demonstrates all four animated classes in the wxAnimator library:

## Classes Demonstrated

### 1. wxStaticTextAnimated
- **Features**: Animated text with font size and color transitions
- **Animation**: Smooth font size scaling from minimum to maximum and back
- **Color**: Foreground color transitions during animation
- **Usage**: Click "Animate Text" button to see the text change and animate

### 2. wxButtonAnimated  
- **Features**: Animated buttons with click effects
- **Animation**: Font size and transparency changes on click
- **Events**: Custom `wxEVT_STATIC_BTN_LEFT_DOWN` event for animation triggers
- **Usage**: Click either animated button to see the animation effect

### 3. wxImageAnimated
- **Features**: Animated images with scaling and rotation effects
- **Animation**: Smooth scaling transformations
- **Flexibility**: Can work with any wxImage
- **Usage**: Click "Animate Image" button to see the image animation

### 4. wxDlgAnimated
- **Features**: Animated dialog window with size transitions
- **Animation**: Smooth window size animations from compact to expanded states
- **Memory Management**: Uses `std::unique_ptr` for safe dialog lifecycle management
- **Usage**: Click "Open Animated Dialog" button to see the dialog animation
- **Safe Closure**: Dialog can be safely opened and closed without application crashes

## Building the Example

### Prerequisites
- CMake 3.16 or higher
- wxWidgets development libraries
- C++17 compatible compiler

### Build Steps

1. **Create build directory**:
   ```bash
   mkdir build
   cd build
   ```

2. **Configure with CMake**:
   ```bash
   cmake ..
   ```

3. **Build the project**:
   ```bash
   make
   ```

4. **Run the example**:
   ```bash
   ./example
   ```

## Example Features

The example application includes:

- **Interactive Demo**: Click buttons to trigger different animations
- **Multiple Animation Types**: Shows text, button, image, and dialog animations
- **Information Panel**: Explains what each class does
- **Clean UI**: Well-organized layout with labeled sections
- **Dialog Demo**: Animated dialog with safe memory management

## How It Works

1. **Text Animation**: When you click "Animate Text", the text content changes and triggers a font size/color animation
2. **Button Animation**: Both buttons animate themselves when clicked using the built-in animation system
3. **Image Animation**: The "Animate Image" button triggers scaling animation on the displayed image
4. **Dialog Animation**: The "Open Animated Dialog" button creates and shows an animated dialog with size transitions and safe memory management

## Code Structure

- `example.cpp`: Main application demonstrating all classes
- `src/wxStaticTextAnimated.cpp`: Animated text implementation
- `src/wxButtonAnimated.cpp`: Animated button implementation  
- `src/wxImageAnimated.cpp`: Animated image implementation
- `src/wxDlgAnimated.cpp`: Animated dialog implementation
- `include/MyWxLib/`: Core animation headers

## Customization

You can customize the animations by modifying:
- Animation duration (`timerDuration_`)
- Font size ranges (`minFontSize_`, `maxFontSize_`)
- Color values in the animation callbacks
- Easing functions for different animation effects

## Troubleshooting

If you encounter build issues:
1. Ensure wxWidgets is properly installed and configured
2. Check that CMake can find the wxWidgets package
3. Verify all include paths are correct in the source files
