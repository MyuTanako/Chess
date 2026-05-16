#include "Checkers.hpp"

void drawPieceSprite(sf::RenderWindow &window, sf::Texture texture, float x, float y)
{
    sf::Sprite sprite(texture);
    float scale;
    sprite.getTexture().getSize().x > sprite.getTexture().getSize().y
                    ? scale = (float)TILE_SIZE / sprite.getTexture().getSize().x
                    : scale = (float)TILE_SIZE / sprite.getTexture().getSize().y;

    // float scaleX = (float)TILE_SIZE / sprite.getTexture().getSize().x;
    // float scaleY = (float)TILE_SIZE / sprite.getTexture().getSize().y;
    sprite.setScale({scale * 1.0f, scale * 1.0f});

    sprite.setPosition({x * TILE_SIZE + TILE_SIZE * 0.05f, y * TILE_SIZE + (TILE_SIZE - scale * sprite.getTexture().getSize().y)/2});
    window.draw(sprite);
}


Checkers::Checkers()
    : currentTurn(PieceColor::White), mustContinueJump(false)
{
    sf::Image blackPieaceImg;
    if(blackPieaceImg.loadFromFile("img/black_cat.png")) 
    {
        blackPieceTex.loadFromImage(blackPieaceImg);
    }

    sf::Image whitePieceImg;
    if(whitePieceImg.loadFromFile("img/white_cat.png")) 
    {
        whitePieceTex.loadFromImage(whitePieceImg);
    }

    sf::Image boardImg;
    if(boardImg.loadFromFile("img/chto-takoe-shahmatnaya-doska.jpg")) 
    {
        boardTex.loadFromImage(boardImg);
    }
    sf::Image gunImg;
    if(gunImg.loadFromFile("img/gun.png")) 
    {
        kingGunTex.loadFromImage(gunImg);
    }
}

Checkers::~Checkers()
{

}

void Checkers::handleClick(int mouseX, int mouseY, sf::Vector2u  winSize)
{
    float scaleX = 800.0f / winSize.x;
    float scaleY = 800.0f / winSize.y;
    int x = static_cast<int>(mouseX * scaleX) / TILE_SIZE;
    int y = static_cast<int>(mouseY * scaleY) / TILE_SIZE;


    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
    {
        return;
    }

    // If nothing selected yet
    if (!selected.has_value())
    {
        const Piece &p = board.getPiece(x, y);
        if (p.color == currentTurn)
        {
            // Check if player has forced jumps
            if (board.hasForcedJumps(currentTurn))
            {
                if(board.hasForcedJumpsForPiece(x, y))
                {
                    selected = {x, y};
                }
            }
            else
            {
                selected = {x, y};
            }
            
        }
        return;
    }

    // We selected and if we click on another of our pieces we change selection
    const Piece &p = board.getPiece(x, y);
    if (!mustContinueJump && p.color == currentTurn)
    {
        if (board.hasForcedJumps(currentTurn))
        {
            if (board.hasForcedJumpsForPiece(x, y))
            {
                selected = {x, y};
            }
        }
        else 
        {
            selected = {x, y};
        }
        return;
    }
    
    // Otherwise try to move
    int fromX = selected->x;
    int fromY = selected->y;

    if (board.isMoveValid(fromX, fromY, x, y, currentTurn))
    {
        auto moveOpt = board.createMove(fromX, fromY, x, y, currentTurn);
        if (moveOpt.has_value()) 
        {
            bool wasJump = moveOpt->capturedPiece.has_value();
            board.makeMove(*moveOpt);

            if (wasJump && board.hasForcedJumpsForPiece(x, y))
            {
                // Must continue jumping
                mustContinueJump = true;
                selected = {x, y};
                return;
            }
            else
            {
                winner = board.checkWin(currentTurn);
                endTurn();
            }
        }
    }
    else
    {
        // Deselect if invalid move and not forced to continue jump
        if (!mustContinueJump)
        {
            selected = std::nullopt;
        }
    }
}

void Checkers::draw(sf::RenderWindow& window)
{
    //Draw board
    sf::Sprite sprite(boardTex);
    sprite.setScale({1.66f, 1.66f});
    // sprite.setPosition({TILE_SIZE + TILE_SIZE * 0.1f, TILE_SIZE + TILE_SIZE * 0.1f});
    window.draw(sprite);


    for (int x = 0; x < BOARD_SIZE; x++)
    {
        for (int y = 0; y < BOARD_SIZE; y++)
        {
            // Highlight selected
            if (selected && selected->x == x && selected->y == y)
            {
                sf::CircleShape hl(TILE_SIZE / 2);
                hl.setPosition({x * (float)TILE_SIZE, y * (float)TILE_SIZE});
                hl.setFillColor(sf::Color(255, 255, 0, 100)); // Yellowish transparent
                window.draw(hl);
            }
            // Draw pieces
            const Piece &p = board.getPiece(x, y); 
            if(!p.isEmpty())
            {
                if(p.color == PieceColor::Black)
                {
                    drawPieceSprite(window, blackPieceTex, x, y);
                }
                if(p.color == PieceColor::White)
                {
                    drawPieceSprite(window, whitePieceTex, x, y);
                }
            }

            if(p.type == PieceType::King)
            {
                sf::Sprite kingGun(kingGunTex);
                float cScaleX = (float)TILE_SIZE / kingGunTex.getSize().x;
                float cScaleY = (float)TILE_SIZE / kingGunTex.getSize().y;
                kingGun.setScale({cScaleX * 0.55f, cScaleY * 0.55f});
                kingGun.setPosition({x * TILE_SIZE + (TILE_SIZE * 0.15f),
                                     y * TILE_SIZE + (TILE_SIZE * 0.30f)});
                window.draw(kingGun);
            }
        }
    }
    
    if(winner != PieceColor::None)
    {
        sf::Font font;
        font.openFromFile("img/font.ttf");
        sf::Text text(font);
        if(winner == PieceColor::Black)
        {
            text.setString("Black won!");
        }
        else
        {
            text.setString("White won!");
        }
        
        text.setCharacterSize(110);
        text.setFillColor(sf::Color::Red);
        text.setPosition({100.f, 300.f});
        window.draw(text);
    }
};

void Checkers::endTurn()
{
  selected = std::nullopt;
  mustContinueJump = false;
  currentTurn = (currentTurn == PieceColor::White) ? PieceColor::Black : PieceColor::White;
}