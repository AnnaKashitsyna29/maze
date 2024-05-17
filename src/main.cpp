#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <random>
#include <regex>

using namespace sf;

const int H = 21;
const int W = 21;

const int ts = 50;

String TileMap[H];

float OffsetX = 0;
float OffsetY = ts * 14;

bool pob = false;
int pe = 2;

bool scoreVisible = false;
bool scoreIsDrawn = true;

class Score {
public:
    Sprite score_sprite[2];
    int score_array[2], score;

    Score(Texture &image) {
        for (int i = 0; i < 2; i++) {
            score_sprite[i].setTexture(image);

            score_sprite[i].setPosition(300 + 25 * i, 5);

            score_sprite[i].setTextureRect(IntRect(0, 0, 25, 36));
        }

        score_array[0] = 0;
        score_array[1] = 0;
    }

    void update() {
        if (!pob)
            score_array[1]++;

        if (score_array[1] >= 10) {
            score_array[0]++;
            score_array[1] = 0;
        }

        for (int i = 0; i < 2; i++) {
            score_sprite[i].setTextureRect(IntRect(25 * score_array[i], 0, 25, 36));
        }
    }
};

class Stopwatch {
public:
    Sprite min[2], sec[2], toc;
    int m[2], s[2], ms;

    Stopwatch(Texture &image) {
        for (int i = 0; i < 2; i++) {
            min[i].setTexture(image);
            sec[i].setTexture(image);

            min[i].setPosition(25 * i, 5);
            sec[i].setPosition(75 + 25 * i, 5);

            min[i].setTextureRect(IntRect(0, 0, 25, 36));
            sec[i].setTextureRect(IntRect(0, 0, 25, 36));

            m[i] = 0;
            s[i] = 0;
        }

        toc.setTexture(image);
        toc.setPosition(50, 5);
        toc.setTextureRect(IntRect(250, 0, 25, 36));

        ms = 0;
    }

    void update() {
        if (!pob && pe == 1)
            ms++;

        if (ms >= 1000) {
            s[1]++;
            ms = 0;
        }

        if (s[1] == 10) {
            s[0]++;
            s[1] = 0;
        }

        if (s[0] == 6) {
            m[1]++;
            s[0] = 0;
        }

        if (m[1] == 10) {
            m[0]++;
            m[1] = 0;
        }

        for (int i = 0; i < 2; i++) {
            sec[i].setTextureRect(IntRect(25 * s[i], 0, 25, 36));
            min[i].setTextureRect(IntRect(25 * m[i], 0, 25, 36));
        }
    }
};

int main() {
    std::ifstream readMap("../../resources/data/map.txt");
    std::list<std::string> strList;
    std::string line;
    int index = 0;
    while (std::getline(readMap, line)) {
        TileMap[index] = line;
        index++;
    }
    readMap.close();

    index = 0;
    srand(time(NULL));
    while (index < 10) {
        int height = rand() % 20 + 1;
        int width = rand() % 20 + 1;
        if (TileMap[width][height] == ' ') {
            TileMap[width][height] = 'T';
            index++;
        }
        else
            continue;
    }

    RenderWindow window(VideoMode(7 * ts, 7 * ts), "Game");

    Texture t;
    t.loadFromFile("C:/Users/turko/Downloads/Game/Paint/Bplat.png");
    Sprite plat(t);

    int x = 1, y = 19;
    int newx = 0, newy = 0;
    TileMap[y][x] = 'B';
    int nx = 2, ny = 5;

    Texture w;
    w.loadFromFile("C:/Users/turko/Downloads/Game/Paint/win.png");
    Sprite win(w);
    win.setPosition(20, 140);

    Texture ti;
    ti.loadFromFile("C:/Users/turko/Downloads/Game/Paint/time.png");
    Stopwatch watch(ti);

    Texture sc;
    sc.loadFromFile("C:/Users/turko/Downloads/Game/Paint/score.png");
    Score score(sc);

    Texture p;
    p.loadFromFile("C:/Users/turko/Downloads/Game/Paint/pause.png");
    Sprite pause(p);
    pause.setPosition(80, 140);


    std::list<sf::Text> textList;
    sf::Font font;
    font.loadFromFile("../../resources/fonts/Pacifico.ttf");
    sf::Text text("", font, 20);
    text.setColor(Color(125, 0, 0));


    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                if (!pob) {
                    if (pe == 1) {
                        newx = x;
                        newy = y;

                        if (event.key.code == Keyboard::Right) {
                            newx += 1;

                            if (TileMap[newy][newx] == ' ' || TileMap[newy][newx] == 'T')
                                nx++;

                            if (nx > 4) {
                                OffsetX += ts;

                                nx = 4;
                            }
                        }
                        if (event.key.code == Keyboard::Left) {
                            newx -= 1;

                            if (TileMap[newy][newx] == ' ' || TileMap[newy][newx] == 'T')
                                nx--;

                            if (nx < 2) {
                                OffsetX -= ts;

                                nx = 2;
                            }
                        }
                        if (event.key.code == Keyboard::Up) {
                            newy -= 1;

                            if (TileMap[newy][newx] == ' ' || TileMap[newy][newx] == 'T')
                                ny--;

                            if (ny < 2) {
                                OffsetY -= ts;
                                ny = 2;
                            }
                        }
                        if (event.key.code == Keyboard::Down) {
                            newy += 1;

                            if (TileMap[newy][newx] == ' ' || TileMap[newy][newx] == 'T')
                                ny++;

                            if (ny > 4) {
                                OffsetY += ts;

                                ny = 4;
                            }
                        }
                    }
                    if (event.key.code == Keyboard::Space) {
                        pe++;

                        if (pe == 3) {
                            pe = 1;
                        }
                    }
                }
                if (event.key.code == Keyboard::Escape) {
                    scoreVisible = !scoreVisible;
                    scoreIsDrawn = false;
                }
            }
        }

        if (TileMap[newy][newx] == ' ') {
            TileMap[y][x] = ' ';

            TileMap[newy][newx] = 'B';

            x = newx;
            y = newy;
        }
        if (TileMap[newy][newx] == 'T') {
            score.update();
            TileMap[y][x] = ' ';
            TileMap[newy][newx] = 'B';

            x = newx;
            y = newy;
            if (score.score_array[0] == 1) {
                std::ifstream readFile("../../resources/data/data.txt");
                std::list<std::string> strList;
                std::string line;
                while (std::getline(readFile, line)) {
                    line = std::regex_replace(line, std::regex(":"), "");
                    strList.push_back(line);
                }
                readFile.close();
                strList.push_back(
                    std::to_string(watch.m[0]) + std::to_string(watch.m[1]) + std::to_string(watch.s[0]) +
                    std::to_string(watch.s[1]));
                strList.sort();
                std::fstream writeFile("../../resources/data/data.txt");
                if (writeFile.is_open()) {
                    int i = 0;
                    for (auto item: strList) {
                        writeFile << item.insert(2, ":") << "\n";
                        i++;
                        if (i == 10)
                            break;
                    }
                }
                writeFile.close();

                pob = true;
            }
        }

        if (OffsetX < 0)
            OffsetX = 0;
        if (OffsetX > ts * 14)
            OffsetX = ts * 14;
        if (OffsetY < 0)
            OffsetY = 0;
        if (OffsetY > ts * 14)
            OffsetY = ts * 14;

        watch.update();
        window.clear(Color::White);
        for (int i = 0; i < H; i++)
            for (int j = 0; j < W; j++) {
                if (TileMap[i][j] == 'A')
                    plat.setTextureRect(IntRect(0, 0, ts, ts));
                if (TileMap[i][j] == 'B')
                    plat.setTextureRect(IntRect(ts, 0, ts, ts));
                if (TileMap[i][j] == 'C')
                    plat.setTextureRect(IntRect(ts * 2, 0, ts, ts));
                if (TileMap[i][j] == 'T')
                    plat.setTextureRect(IntRect(ts * 3, 0, ts, ts));
                if (TileMap[i][j] == ' ')
                    continue;

                plat.setPosition(j * ts - OffsetX, i * ts - OffsetY);
                window.draw(plat);
            }
        if (pob)
            window.draw(win);
        for (int i = 0; i < 2; i++) {
            window.draw(watch.min[i]);
            window.draw(watch.sec[i]);

            window.draw(score.score_sprite[i]);
        }
        window.draw(watch.toc);
        if (pe == 2)
            window.draw(pause);
        if (scoreVisible) {
            window.clear(Color::Black);
            std::ifstream readFile("../../resources/data/data.txt");
            std::string line;
            text.setString("Top score:");
            textList.push_back(text);
            while (std::getline(readFile, line)) {
                text.setString(line);
                textList.push_back(text);
            }
            readFile.close();
            float i = 0;
            for (std::list<sf::Text>::iterator it = textList.begin(); it != textList.end(); ++it) {
                sf::Text &text = *it;
                sf::FloatRect boundings = text.getLocalBounds();
                text.setPosition(window.getSize().x / 2 - text.getGlobalBounds().width / 2, i * (boundings.height + 7));
                ++i;
                window.draw(text);
            }
            textList = {};
            scoreIsDrawn = true;
        }
        window.display();
    }

    return 0;
}
