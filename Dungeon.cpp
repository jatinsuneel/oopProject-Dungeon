#include "Dungeon.h"
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

static const int TILE = 46;

Dungeon::Dungeon() {
    currentLevel = 0;

    levels = {
        {
            {'#','#','#','#','#','#','#','#','#'},
            {'#','.','.','.','.','.','.','.','#'},
            {'#','.','#','#','.','#','.','.','#'},
            {'#','.','.','.','.','.','.','E','#'},
            {'#','.','#','.','#','#','.','.','#'},
            {'#','.','.','.','.','.','.','.',  '#'},
            {'#','.','.','.',  'T','.','.','.',  '#'},
            {'#','#','#','#','#','#','#','#','#'}
        },
        {
            {'#','#','#','#','#','#','#','#','#','#'},
            {'#','.','.','.','#','.','.','.','E','#'},
            {'#','.','#','.','#','.','#','.','.','#'},
            {'#','.','#','.','.','.',  '#','.','.','#'},
            {'#','.','.','.',  '#','.','.','.',  '#','#'},
            {'#','.','#','.','#','#','.','T','.','#'},
            {'#','.','.','.','.','.','.','.','.','#'},
            {'#','#','#','#','#','#','#','#','#','#'}
        },
        {
            {'#','#','#','#','#','#','#','#','#','#','#','#'},
            {'#','.','#','.','.','.',  '#','.','.','.',  'E','#'},
            {'#','.','#','.','#','.','#','.','#','#','.','#'},
            {'#','.','.','.',  '#','.','.','.',  '#','.','.','#'},
            {'#','#','#','.','#','#','#','.','#','.','#','#'},
            {'#','.','.','.','.','.',  '#','.','.','.','.','#'},
            {'#','.','#','#','#','.','#','#','#','#','.','#'},
            {'#','.','#','T','.','.','.','.','.',  '#','.','#'},
            {'#','.','#','#','#','#','#','#','.','#','.','#'},
            {'#','.','.','.','.','.','.','.','.','.','.','#'},
            {'#','#','#','#','#','#','#','#','#','#','#','#'}
        },
        {
            {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
            {'#','.','.','.','#','.','.','.','.','.','#','.','.','E','#'},
            {'#','.','#','.','#','.','#','#','#','.','#','.','#','.','#'},
            {'#','.','#','.','.','.','#','.','#','.','.','.','#','.','#'},
            {'#','.','#','#','#','.','#','.','#','#','#','.','#','.','#'},
            {'#','.','.','.','#','.','.','.','.','.','#','.','#','.','#'},
            {'#','#','#','.','#','#','#','#','#','.','#','.','#','.','#'},
            {'#','T','#','.','.','.','.','.','#','.','.','.','#','.','#'},
            {'#','.','#','#','#','#','#','.','#','#','#','#','#','.','#'},
            {'#','.','.','.','.','.','#','.','.','.','.','.','.','.','#'},
            {'#','.','#','#','#','.','#','#','#','#','#','#','#','.','#'},
            {'#','.','.','.','#','.','.','.','.','.','.','.','#','.','#'},
            {'#','#','#','.','#','#','#','#','#','#','#','.','#','.','#'},
            {'#','.','.','.','.','.','.','.','.','.','#','.','.','.','#'},
            {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
        }
    };
}

int Dungeon::getCurrentLevelNumber() const { return currentLevel + 1; }
int Dungeon::getTotalLevels() const { return (int)levels.size(); }
bool Dungeon::isLastLevel() const { return currentLevel == (int)levels.size() - 1; }

void Dungeon::nextLevel() {
    if (!isLastLevel()) currentLevel++;
}

bool Dungeon::isWall(int x, int y) const {
    if (y < 0 || y >= getHeight() || x < 0 || x >= getWidth()) return true;
    return levels[currentLevel][y][x] == '#';
}

char Dungeon::getCell(int x, int y) const { return levels[currentLevel][y][x]; }
void Dungeon::setCell(int x, int y, char value) { levels[currentLevel][y][x] = value; }
int Dungeon::getWidth() const { return (int)levels[currentLevel][0].size(); }
int Dungeon::getHeight() const { return (int)levels[currentLevel].size(); }

void Dungeon::display(const Player& player,
                      const vector<Monster>& monsters,
                      sf::RenderWindow& window,
                      sf::Font& font,
                      const string& playerName) const {

    sf::Vector2u winSize = window.getSize();
    float W = (float)winSize.x;
    float H = (float)winSize.y;

    int cols = getWidth();
    int rows = getHeight();

    // --- Background gradient ---
    sf::RectangleShape bg({W, H});
    bg.setFillColor(sf::Color(12, 12, 24));
    window.draw(bg);

    // --- Calculate tile size and offset to center dungeon ---
    int tileW = TILE;
    int tileH = TILE;

    float dungeonPixelW = (float)(cols * tileW);
    float dungeonPixelH = (float)(rows * tileH);

    float hudHeight = 70.f;
    float controlsHeight = 50.f;
    float availableH = H - hudHeight - controlsHeight;

    // Fit dungeon to available space if too large
    if (dungeonPixelW > W - 40.f || dungeonPixelH > availableH - 10.f) {
        float scaleX = (W - 40.f) / dungeonPixelW;
        float scaleY = (availableH - 10.f) / dungeonPixelH;
        float scale = min(scaleX, scaleY);
        tileW = (int)(tileW * scale);
        tileH = (int)(tileH * scale);
        dungeonPixelW = (float)(cols * tileW);
        dungeonPixelH = (float)(rows * tileH);
    }

    float offsetX = (W - dungeonPixelW) / 2.f;
    float offsetY = hudHeight + (availableH - dungeonPixelH) / 2.f;

    // --- Draw tiles ---
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            float px = offsetX + col * tileW;
            float py = offsetY + row * tileH;

            char cell = levels[currentLevel][row][col];

            // Floor or wall base
            sf::RectangleShape tile({(float)tileW - 1.f, (float)tileH - 1.f});
            tile.setPosition({px, py});

            if (cell == '#') {
                tile.setFillColor(sf::Color(35, 35, 58));
                tile.setOutlineColor(sf::Color(20, 20, 38));
                tile.setOutlineThickness(1.f);
                window.draw(tile);

                // Inner shadow bevel
                sf::RectangleShape bevel({(float)tileW - 5.f, (float)tileH - 5.f});
                bevel.setPosition({px + 2.f, py + 2.f});
                bevel.setFillColor(sf::Color(45, 45, 70));
                window.draw(bevel);
            } else {
                tile.setFillColor(sf::Color(22, 22, 38));
                tile.setOutlineColor(sf::Color(30, 30, 50));
                tile.setOutlineThickness(1.f);
                window.draw(tile);
            }

            // Special cells
            if (cell == 'T') {
                float pad = tileW * 0.18f;
                sf::RectangleShape gem({(float)tileW - pad * 2.f, (float)tileH - pad * 2.f});
                gem.setPosition({px + pad, py + pad});
                gem.setFillColor(sf::Color(200, 160, 0));
                gem.setOutlineColor(sf::Color(255, 220, 60));
                gem.setOutlineThickness(2.f);
                window.draw(gem);

                sf::Text t(font, "T", (unsigned int)(tileH * 0.5f));
                t.setFillColor(sf::Color(255, 240, 100));
                t.setStyle(sf::Text::Bold);
                sf::FloatRect tb = t.getLocalBounds();
                t.setOrigin({tb.size.x / 2.f, tb.size.y / 2.f});
                t.setPosition({px + tileW / 2.f, py + tileH / 2.f});
                window.draw(t);

            } else if (cell == 'E') {
                float pad = tileW * 0.15f;
                sf::RectangleShape exitShape({(float)tileW - pad * 2.f, (float)tileH - pad * 2.f});
                exitShape.setPosition({px + pad, py + pad});
                exitShape.setFillColor(sf::Color(0, 160, 200));
                exitShape.setOutlineColor(sf::Color(80, 220, 255));
                exitShape.setOutlineThickness(2.f);
                window.draw(exitShape);

                sf::Text t(font, "E", (unsigned int)(tileH * 0.5f));
                t.setFillColor(sf::Color(180, 240, 255));
                t.setStyle(sf::Text::Bold);
                sf::FloatRect tb = t.getLocalBounds();
                t.setOrigin({tb.size.x / 2.f, tb.size.y / 2.f});
                t.setPosition({px + tileW / 2.f, py + tileH / 2.f});
                window.draw(t);
            }
        }
    }

    // --- Draw monsters ---
    for (const Monster& m : monsters) {
        float px = offsetX + m.getX() * tileW;
        float py = offsetY + m.getY() * tileH;

        float pad = tileW * 0.1f;
        sf::RectangleShape body({(float)tileW - pad * 2.f, (float)tileH - pad * 2.f});
        body.setPosition({px + pad, py + pad});
        body.setFillColor(sf::Color(180, 30, 30));
        body.setOutlineColor(sf::Color(255, 80, 80));
        body.setOutlineThickness(2.f);
        window.draw(body);

        sf::Text sym(font, "M", (unsigned int)(tileH * 0.48f));
        sym.setFillColor(sf::Color(255, 200, 200));
        sym.setStyle(sf::Text::Bold);
        sf::FloatRect sb = sym.getLocalBounds();
        sym.setOrigin({sb.size.x / 2.f, sb.size.y / 2.f});
        sym.setPosition({px + tileW / 2.f, py + tileH / 2.f});
        window.draw(sym);
    }

    // --- Draw player ---
    {
        float px = offsetX + player.getX() * tileW;
        float py = offsetY + player.getY() * tileH;

        float pad = tileW * 0.08f;
        sf::RectangleShape body({(float)tileW - pad * 2.f, (float)tileH - pad * 2.f});
        body.setPosition({px + pad, py + pad});
        body.setFillColor(sf::Color(30, 180, 80));
        body.setOutlineColor(sf::Color(100, 255, 150));
        body.setOutlineThickness(2.f);
        window.draw(body);

        // Player symbol P
        sf::Text sym(font, "P", (unsigned int)(tileH * 0.45f));
        sym.setFillColor(sf::Color(200, 255, 220));
        sym.setStyle(sf::Text::Bold);
        sf::FloatRect sb = sym.getLocalBounds();
        sym.setOrigin({sb.size.x / 2.f, sb.size.y / 2.f});
        sym.setPosition({px + tileW / 2.f, py + tileH * 0.42f});
        window.draw(sym);

        // Player name tag above
        if (!playerName.empty()) {
            sf::Text nameTag(font, playerName, (unsigned int)(tileH * 0.28f));
            nameTag.setFillColor(sf::Color(180, 255, 200));
            sf::FloatRect nb = nameTag.getLocalBounds();
            nameTag.setOrigin({nb.size.x / 2.f, nb.size.y});
            nameTag.setPosition({px + tileW / 2.f, py - 4.f});

            // Name tag background
            sf::RectangleShape tagBg({nb.size.x + 8.f, nb.size.y + 4.f});
            tagBg.setFillColor(sf::Color(0, 0, 0, 160));
            tagBg.setPosition({px + tileW / 2.f - nb.size.x / 2.f - 4.f, py - nb.size.y - 8.f});
            window.draw(tagBg);
            window.draw(nameTag);
        }
    }

    // --- HUD top bar ---
    sf::RectangleShape hudBg({W, hudHeight});
    hudBg.setFillColor(sf::Color(8, 8, 20));
    hudBg.setOutlineColor(sf::Color(0, 180, 255, 80));
    hudBg.setOutlineThickness(1.f);
    hudBg.setPosition({0.f, 0.f});
    window.draw(hudBg);

    // Name left
    sf::Text nameHud(font, "[ " + playerName + " ]", 20);
    nameHud.setFillColor(sf::Color(0, 210, 255));
    nameHud.setStyle(sf::Text::Bold);
    nameHud.setPosition({16.f, 20.f});
    window.draw(nameHud);

    // HP center
    int hp = player.getHealth();
    sf::Color hpColor = hp > 60 ? sf::Color(60, 220, 80)
                      : hp > 30 ? sf::Color(240, 180, 0)
                                : sf::Color(230, 50, 50);
    sf::Text hpText(font, "HP: " + to_string(hp), 20);
    hpText.setFillColor(hpColor);
    hpText.setStyle(sf::Text::Bold);
    sf::FloatRect hb = hpText.getLocalBounds();
    hpText.setOrigin({hb.size.x / 2.f, 0.f});
    hpText.setPosition({W / 2.f, 20.f});
    window.draw(hpText);

    // Level right
    string lvlStr = "Level " + to_string(getCurrentLevelNumber()) + " / " + to_string(getTotalLevels());
    sf::Text lvlText(font, lvlStr, 20);
    lvlText.setFillColor(sf::Color(200, 160, 255));
    lvlText.setStyle(sf::Text::Bold);
    sf::FloatRect lb = lvlText.getLocalBounds();
    lvlText.setOrigin({lb.size.x, 0.f});
    lvlText.setPosition({W - 16.f, 20.f});
    window.draw(lvlText);

    // --- Controls bar bottom ---
    sf::RectangleShape ctrlBg({W, controlsHeight});
    ctrlBg.setFillColor(sf::Color(8, 8, 20));
    ctrlBg.setOutlineColor(sf::Color(0, 180, 255, 60));
    ctrlBg.setOutlineThickness(1.f);
    ctrlBg.setPosition({0.f, H - controlsHeight});
    window.draw(ctrlBg);

    sf::Text ctrlText(font, "WASD: Move   U: Undo   I: Inventory   Q: Quit", 16);
    ctrlText.setFillColor(sf::Color(130, 130, 180));
    sf::FloatRect cb = ctrlText.getLocalBounds();
    ctrlText.setOrigin({cb.size.x / 2.f, cb.size.y / 2.f});
    ctrlText.setPosition({W / 2.f, H - controlsHeight / 2.f});
    window.draw(ctrlText);

    // HP bar below name in HUD
    float barW = 180.f;
    float barH = 8.f;
    float barX = 16.f;
    float barY = hudHeight - 18.f;

    sf::RectangleShape barBg({barW, barH});
    barBg.setFillColor(sf::Color(50, 20, 20));
    barBg.setPosition({barX, barY});
    window.draw(barBg);

    float hpRatio = (float)hp / 100.f;
    sf::RectangleShape barFill({barW * hpRatio, barH});
    barFill.setFillColor(hpColor);
    barFill.setPosition({barX, barY});
    window.draw(barFill);
}