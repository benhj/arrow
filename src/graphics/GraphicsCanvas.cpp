// GraphicsCanvas.cpp
// This version uses SFML (Simple and Fast Multimedia Library)
// This has been re-architected from the olcPixelGameEngine version to use SFML.

#include "GraphicsCanvas.hpp"
#include <iostream> // For debugging, if needed

namespace arrow // Start of the arrow namespace for implementations
{

// Initialize static members
std::vector<std::function<void(sf::RenderWindow&)>> GraphicsCanvas::s_drawOperations;
std::mutex GraphicsCanvas::s_drawOperationsMutex;

// Private constructor implementation
GraphicsCanvas::GraphicsCanvas() : m_window(nullptr)
{
    // Initialize SFML window here if needed, or in construct()
}

// Destructor implementation
GraphicsCanvas::~GraphicsCanvas()
{
    if (m_window)
    {
        m_window->close(); // Ensure the window is closed
    }
}

// Static method to get the single instance
GraphicsCanvas& GraphicsCanvas::getInstance()
{
    static GraphicsCanvas instance;
    return instance;
}

// Static construct method implementation
bool GraphicsCanvas::construct(int32_t width, int32_t height, int32_t pixel_width, int32_t pixel_height)
{
    GraphicsCanvas& instance = getInstance();

    if (instance.m_window && instance.m_window->isOpen()) {
        std::cerr << "GraphicsCanvas already constructed!" << std::endl;
        return false; // Already constructed
    }

    // SFML 3.x: sf::VideoMode constructor now takes sf::Vector2u
    instance.m_window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(sf::Vector2u(static_cast<unsigned int>(width), static_cast<unsigned int>(height))),
        "GraphicsCanvas",
        sf::Style::Default
    );

    if (!instance.m_window->isOpen())
    {
        std::cerr << "Failed to create SFML window!" << std::endl;
        return false;
    }

    // Optional: Set vertical sync for smoother animation
    instance.m_window->setVerticalSyncEnabled(true);

    std::cout << "SFML GraphicsCanvas constructed with dimensions: " << width << "x" << height << std::endl;
    // pixel_width/height are ignored, but kept in signature for API compatibility.
    // If pixel scaling is desired, an sf::RenderTexture approach would be needed.

    return true;
}

// Static method to process one frame
bool GraphicsCanvas::process_frame()
{
    GraphicsCanvas& instance = getInstance();

    if (!instance.m_window || !instance.m_window->isOpen())
    {
        return false; // Window is not open or not constructed
    }

    // SFML 3.x event polling with std::optional.
    // The event type is now accessed using is<sf::Event::Closed>()
    std::optional<sf::Event> event_opt;
    while ((event_opt = instance.m_window->pollEvent()))
    {
        if (event_opt.has_value()) {
            const sf::Event& event = event_opt.value(); // Get the actual event object

            // Use event.is<sf::Event::Closed>() for SFML 3.x
            // This is the cleanest way to check the event type in the new variant-based system.
            if (event.is<sf::Event::Closed>())
            {
                instance.m_window->close();
                return false; // Window closed by user
            }
            // If you need to access event data for other types, you'd use get<EventType>()
            // Example for KeyPressed:
            // else if (event.is<sf::Event::KeyPressed>()) {
            //     const auto& key_event = event.get<sf::Event::KeyPressed>();
            //     // Now you can access key_event.code, key_event.alt, etc.
            // }
        }
    }

    // Clear the screen
    instance.m_window->clear(sf::Color::Black);

    // Execute queued drawing operations
    std::lock_guard<std::mutex> lock(s_drawOperationsMutex);
    for (const auto& drawFunc : s_drawOperations)
    {
        drawFunc(*instance.m_window); // Pass the window to the drawing lambda
    }
    s_drawOperations.clear(); // Clear operations after drawing

    // Display what's been drawn
    instance.m_window->display();

    return true; // Keep the engine running
}

// Drawing implementations using SFML primitives

void GraphicsCanvas::draw(int32_t const x, int32_t const y, int const r, int const g, int const b)
{
    std::lock_guard<std::mutex> lock(s_drawOperationsMutex);
    sf::Color color(static_cast<std::uint8_t>(r), static_cast<std::uint8_t>(g), static_cast<std::uint8_t>(b));
    s_drawOperations.push_back([x, y, color](sf::RenderWindow& window) {
        sf::RectangleShape pixel(sf::Vector2f(1.0f, 1.0f));
        pixel.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        pixel.setFillColor(color);
        window.draw(pixel);
    });
}

void GraphicsCanvas::drawLine(int32_t const x1, int32_t const y1, int32_t const x2, int32_t const y2, int const r, int const g, int const b)
{
    std::lock_guard<std::mutex> lock(s_drawOperationsMutex);
    sf::Color color(static_cast<std::uint8_t>(r), static_cast<std::uint8_t>(g), static_cast<std::uint8_t>(b));
    s_drawOperations.push_back([x1, y1, x2, y2, color](sf::RenderWindow& window) {
        // Construct sf::Vertex using default constructor then set members
        sf::Vertex line_points[2];
        line_points[0].position = sf::Vector2f(static_cast<float>(x1), static_cast<float>(y1));
        line_points[0].color = color;
        line_points[1].position = sf::Vector2f(static_cast<float>(x2), static_cast<float>(y2));
        line_points[1].color = color;

        window.draw(line_points, 2, sf::PrimitiveType::Lines);
    });
}

void GraphicsCanvas::drawCircle(int32_t const x, int32_t const y, int32_t const radius, int const r, int const g, int const b)
{
    std::lock_guard<std::mutex> lock(s_drawOperationsMutex);
    sf::Color color(static_cast<std::uint8_t>(r), static_cast<std::uint8_t>(g), static_cast<std::uint8_t>(b));
    s_drawOperations.push_back([x, y, radius, color](sf::RenderWindow& window) {
        sf::CircleShape circle(static_cast<float>(radius));
        circle.setPosition(sf::Vector2f(static_cast<float>(x - radius), static_cast<float>(y - radius)));
        circle.setFillColor(sf::Color::Transparent); // For drawing outline only
        circle.setOutlineColor(color);
        circle.setOutlineThickness(1.0f);
        window.draw(circle);
    });
}

void GraphicsCanvas::fillCircle(int32_t const x, int32_t const y, int32_t const radius, int const r, int const g, int const b)
{
    std::lock_guard<std::mutex> lock(s_drawOperationsMutex);
    sf::Color color(static_cast<std::uint8_t>(r), static_cast<std::uint8_t>(g), static_cast<std::uint8_t>(b));
    s_drawOperations.push_back([x, y, radius, color](sf::RenderWindow& window) {
        sf::CircleShape circle(static_cast<float>(radius));
        circle.setPosition(sf::Vector2f(static_cast<float>(x - radius), static_cast<float>(y - radius)));
        circle.setFillColor(color);
        window.draw(circle);
    });
}

void GraphicsCanvas::drawRect(int32_t const x, int32_t const y, int32_t const w, int32_t const h, int const r, int const g, int const b)
{
    std::lock_guard<std::mutex> lock(s_drawOperationsMutex);
    sf::Color color(static_cast<std::uint8_t>(r), static_cast<std::uint8_t>(g), static_cast<std::uint8_t>(b));
    s_drawOperations.push_back([x, y, w, h, color](sf::RenderWindow& window) {
        sf::RectangleShape rect(sf::Vector2f(static_cast<float>(w), static_cast<float>(h)));
        rect.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        rect.setFillColor(sf::Color::Transparent); // For drawing outline only
        rect.setOutlineColor(color);
        rect.setOutlineThickness(1.0f);
        window.draw(rect);
    });
}

void GraphicsCanvas::fillRect(int32_t const x, int32_t const y, int32_t const w, int32_t const h, int const r, int const g, int const b)
{
    std::lock_guard<std::mutex> lock(s_drawOperationsMutex);
    sf::Color color(static_cast<std::uint8_t>(r), static_cast<std::uint8_t>(g), static_cast<std::uint8_t>(b));
    s_drawOperations.push_back([x, y, w, h, color](sf::RenderWindow& window) {
        sf::RectangleShape rect(sf::Vector2f(static_cast<float>(w), static_cast<float>(h)));
        rect.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        rect.setFillColor(color);
        window.draw(rect);
    });
}

} // End of the arrow namespace