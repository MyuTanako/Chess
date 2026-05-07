#include "Checkers.hpp"

void drawPieceSpeite(sf::RenderWindow &window, sf::Texture texture, float x, float y)
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

    // Board board();
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
        return;

    // If nothing selected yet
    if (!selected.has_value())
    {
        const Piece &p = board.getPieceAt(x, y);
        if (p.color == currentTurn)
        {
            selected = {x, y};
        }
        else
        {

        }
        return;
    }

    // We selected and if we click on another of our pieces we change selection
    const Piece &p = board.getPieceAt(x, y);
    if (/*!mustContinueJump && */p.color == currentTurn)
    {
        selected = {x, y};
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


            endTurn();
        }
    }



}

void Checkers::draw(sf::RenderWindow& window)
{
    // for(int x = 0; x < BOARD_SIZE; x++)
    // {
    //     for (int y = 0; y < BOARD_SIZE; y++)
    //     {
    //         sf::Texture sqTex;
            //check for even
            // if((x + y) % 2 == 0)
            // {
            //     sqTex(darkSqTex);

            // }
            // else
            // {
            //     sqTex(darkSqTex);


            // }
    //         sf::Sprite sprite(sqTex);
    //         sprite.setPosition({x * TILE_SIZE + TILE_SIZE * 0.1f,
    //                        y * TILE_SIZE + TILE_SIZE * 0.1f});
    //     }
    // }


    //Draw board
    sf::Sprite sprite(boardTex);
    sprite.setScale({1.66f, 1.66f});
    // sprite.setPosition({TILE_SIZE + TILE_SIZE * 0.1f, TILE_SIZE + TILE_SIZE * 0.1f});
    window.draw(sprite);

    // Highlight selected

    // Draw pieces
    for (int x = 0; x < BOARD_SIZE; x++)
    {
        for (int y = 0; y < BOARD_SIZE; y++)
        {
            const Piece &p = board.getPieceAt(x, y);
            
            if(p.isEmpty())
            {
                drawPieceSpeite(window, blackPieceTex, x, y);
                if(p.color == PieceColor::Black)
                {
                    drawPieceSpeite(window, blackPieceTex, x, y);
                }
                if(p.color == PieceColor::White)
                {
                    drawPieceSpeite(window, whitePieceTex, x, y);
                }
            }
        }
        
    }

    

};

void Checkers::endTurn()
{
  selected = std::nullopt;
  /*mustContinueJump = false;*/
  currentTurn = (currentTurn == PieceColor::White) ? PieceColor::Black : PieceColor::White;
}