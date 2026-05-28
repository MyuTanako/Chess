#pragma once

enum class PieceColor { None, White, Black };
enum class PieceType { Man, King };

struct Piece {
    PieceColor color = PieceColor::None;
    PieceType type = PieceType::Man;

    bool isEmpty() const
    {
        return this->color == PieceColor::None;
    }
};