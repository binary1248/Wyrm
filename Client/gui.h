#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFGUI/GUI.hpp>

class WidgetSet{
  public:
    typedef boost::shared_ptr<WidgetSet> Ptr;

    static WidgetSet::Ptr Create(const sf::FloatRect rect, const std::string name);

    ~WidgetSet();

    void AddWidget(sfg::Widget::Ptr widget);

    void Draw(sf::RenderWindow& w);

    bool HandleEvent(sf::Event& e);

    sf::FloatRect GetRect();

    sfg::Widget::Ptr FindWidget(const std::string& id);

    inline void Show() {active = true;}
    inline void Hide() {active = false;}
    inline bool IsActive() {return active;}
    inline std::string GetName() {return name;}
  private:
    WidgetSet(const sf::FloatRect rect, const std::string name);

    sf::Shape background;
    sfg::GUI* gui;
    std::string name;
    bool active;
};

void LoadGUI(sf::RenderWindow& w);

void DrawGUI(sf::RenderWindow& w);

bool HandleGUIEvent(sf::Event& e);

extern bool bRunning;

#endif // GUI_H_INCLUDED
