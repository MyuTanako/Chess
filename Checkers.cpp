#include "Checkers.hpp"


Checkers::Checkers()
    : currentTurn(PieceColor::White), mustContinueJump(false)
{
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

};

void Checkers::endTurn()
{
  selected = std::nullopt;
  /*mustContinueJump = false;*/
  currentTurn = (currentTurn == PieceColor::White) ? PieceColor::Black : PieceColor::White;
}