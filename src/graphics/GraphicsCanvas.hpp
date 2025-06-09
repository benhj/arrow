// GraphicsCanvas.hpp
// Note: This version uses SFML (Simple and Fast Multimedia Library)
// Please see https://www.sfml-dev.org/
// This has been re-architected from the olcPixelGameEngine version to use SFML.

#ifndef GRAPHICS_CANVAS_HPP
#define GRAPHICS_CANVAS_HPP

#include <SFML/Graphics.hpp>    // For sf::RenderWindow, sf::Color, sf::Shape, etc.
#include <SFML/Window.hpp>      // For window events
#include <SFML/System.hpp>      // For sf::Clock, sf::Time

#include <vector>               // For std::vector
#include <functional>           // For std::function
#include <mutex>                // For std::mutex
#include <memory>               // For std::unique_ptr
#include <cstdint>              // For int32_t, uint8_t

namespace arrow // Start of the arrow namespace
{

class GraphicsCanvas
{
private:
    // Private constructor for Singleton
    GraphicsCanvas();
    // Delete copy and move constructors/assignment operators
    GraphicsCanvas(const GraphicsCanvas&) = delete;
    GraphicsCanvas& operator=(const GraphicsCanvas&) = delete;
    GraphicsCanvas(GraphicsCanvas&&) = delete;
    GraphicsCanvas& operator=(GraphicsCanvas&&) = delete;

public:
    // Destructor
    ~GraphicsCanvas();

    // Static method to get the single instance of GraphicsCanvas
    static GraphicsCanvas& getInstance();

    // Static API to construct the engine. It will NOT start a blocking loop.
    // The main loop will be driven externally by process_frame().
    // pixel_width and pixel_height are ignored as SFML uses direct window dimensions.
    static bool construct(int32_t width, int32_t height, int32_t pixel_width = 1, int32_t pixel_height = 1);

    // Static method to process one frame of the graphics canvas.
    // This must be called repeatedly by the main loop of your toy language.
    // Returns true if the canvas is still active, false if it should close.
    static bool process_frame();

    // Static Public API for drawing
    static void draw(int32_t const x, int32_t const y, int const r, int const g, int const b);
    static void drawLine(int32_t const x1, int32_t const y1, int32_t const x2, int32_t const y2, int const r, int const g, int const b);
    static void drawCircle(int32_t const x, int32_t const y, int32_t const radius, int const r, int const g, int const b);
    static void fillCircle(int32_t const x, int32_t const y, int32_t const radius, int const r, int const g, int const b);
    static void drawRect(int32_t const x, int32_t const y, int32_t const w, int32_t const h, int const r, int const g, int const b);
    static void fillRect(int32_t const x, int32_t const y, int32_t const w, int32_t const h, int const r, int const g, int const b);

private:
    // Internal SFML window instance
    std::unique_ptr<sf::RenderWindow> m_window;

    // Drawing operations queue
    static std::vector<std::function<void(sf::RenderWindow&)>> s_drawOperations;
    static std::mutex s_drawOperationsMutex;
};

} // End of the arrow namespace

#endif // GRAPHICS_CANVAS_HPP