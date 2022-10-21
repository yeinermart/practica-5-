#include "game.h"
#define W (GameObject::Width)

Pacman::Pacman() : GameObject(
    GameObject::Pacman, QPixmap(":/n/pacman/a3.png"))
{
    dir = Stop;
    next_dir = Stop;
    anim_index = 2;
    anim[Right].push_back(QPixmap(":/n/pacman/a1.png"));  //agrege al final del vector
    anim[Right].push_back(QPixmap(":/n/pacman/a2.png"));
    anim[Right].push_back(QPixmap(":/n/pacman/a3.png"));
    anim[Right].push_back(QPixmap(":/n/pacman/a4.png"));
    anim[Right].push_back(QPixmap(":/n/pacman/a5.png"));
    anim[Right].push_back(QPixmap(":/n/pacman/a6.png"));
    anim[Right].push_back(QPixmap(":/n/pacman/a5.png"));
    anim[Right].push_back(QPixmap(":/n/pacman/a4.png"));
    anim[Right].push_back(QPixmap(":/n/pacman/a3.png"));
    anim[Right].push_back(QPixmap(":/n/pacman/a2.png"));

    anim[Up].push_back(QPixmap(":/n/pacman/a1.png"));
    anim[Up].push_back(QPixmap(":/n/pacman/b2.png"));
    anim[Up].push_back(QPixmap(":/n/pacman/b3.png"));
    anim[Up].push_back(QPixmap(":/n/pacman/b4.png"));
    anim[Up].push_back(QPixmap(":/n/pacman/b5.png"));
    anim[Up].push_back(QPixmap(":/n/pacman/b6.png"));
    anim[Up].push_back(QPixmap(":/n/pacman/b5.png"));
    anim[Up].push_back(QPixmap(":/n/pacman/b4.png"));
    anim[Up].push_back(QPixmap(":/n/pacman/b3.png"));
    anim[Up].push_back(QPixmap(":/n/pacman/b2.png"));

    anim[Left].push_back(QPixmap(":/n/pacman/a1.png"));
    anim[Left].push_back(QPixmap(":/n/pacman/c2.png"));
    anim[Left].push_back(QPixmap(":/n/pacman/c3.png"));
    anim[Left].push_back(QPixmap(":/n/pacman/c4.png"));
    anim[Left].push_back(QPixmap(":/n/pacman/c5.png"));
    anim[Left].push_back(QPixmap(":/n/pacman/c6.png"));
    anim[Left].push_back(QPixmap(":/n/pacman/c5.png"));
    anim[Left].push_back(QPixmap(":/n/pacman/c4.png"));
    anim[Left].push_back(QPixmap(":/n/pacman/c3.png"));
    anim[Left].push_back(QPixmap(":/n/pacman/c2.png"));

    anim[Down].push_back(QPixmap(":/n/pacman/a1.png"));
    anim[Down].push_back(QPixmap(":/n/pacman/d2.png"));
    anim[Down].push_back(QPixmap(":/n/pacman/d3.png"));
    anim[Down].push_back(QPixmap(":/n/pacman/d4.png"));
    anim[Down].push_back(QPixmap(":/n/pacman/d5.png"));
    anim[Down].push_back(QPixmap(":/n/pacman/d6.png"));
    anim[Down].push_back(QPixmap(":/n/pacman/d5.png"));
    anim[Down].push_back(QPixmap(":/n/pacman/d4.png"));
    anim[Down].push_back(QPixmap(":/n/pacman/d3.png"));
    anim[Down].push_back(QPixmap(":/n/pacman/d2.png"));
}

void Pacman::moveup()    //mover arriba
{
    anim_index++;
    if (anim_index == anim[Up].size()) {
        anim_index = 0;
    }
    setPixmap(anim[Up][anim_index]);
    setY(static_cast<int>(y()) - 1);
}

void Pacman::moveleft()   //izquierda
{
    anim_index++;
    if (anim_index == anim[Left].size()) {
        anim_index = 0;
    }
    setPixmap(anim[Left][anim_index]);
    setX(static_cast<int>(x()) - 1);
}

void Pacman::movedown()   //abajo
{
    anim_index++;
    if (anim_index == anim[Down].size()) {
        anim_index = 0;
    }
    setPixmap(anim[Down][anim_index]);
    setY(static_cast<int>(y()) + 1);
}

void Pacman::moveright()  //derecha
{
    anim_index++;
    if (anim_index == anim[Right].size()) {
        anim_index = 0;
    }
    setPixmap(anim[Right][anim_index]);
    setX(static_cast<int>(x()) + 1);
}


void Pacman::eat_ball(int __y, int __x) //comer pelota
{
    GameObject *obj = game->map[__y][__x];
    switch (obj->get_type()) {
    case Ball:                           //come pelota
        game->score += obj->get_score();   //actualiza puntaje
        game->ball_num--;                  //dismnuye el # pelotas
        break;
    case PowerBall:                      //come super pelota
        game->score += obj->get_score();
        game->ball_num--;
        for (int i = 0; i < game->powerball.size(); i++) {
            if (game->powerball.at(i) == obj) {
                game->powerball.remove(i);
                break;
            }
        }
        break;
    default:
        return;
    }

    /* Pacman se come una pelota, y
     * llenar el bloque anterior con espacios en blanco. */
    QPixmap blankpix;
    game->map[_y][_x] = new GameObject(GameObject::Blank, blankpix);
    game->map[__y][__x] = this;
    delete obj;
}


bool Pacman::overlapable(int i, int j)
{
    if (i < 0 || j < 0) {
        return false;
    }

    if (i >= game->map_height || j >= game->map_width) {
        return false;
    }

    switch (game->map[i][j]->get_type()) {
    case Wall:
    case Gate:
        return false;
    default:
        return true;
    }
}


void Pacman::move()
{
    int pac_x = static_cast<int>(x());
    int pac_y = static_cast<int>(y());
    int __x = (pac_x - game->geo_x) / W;            // x coordinar en el mapa
    int __y = (pac_y - game->geo_y) / W;            // y coordinar en el mapa
    int x_remainder = (pac_x - game->geo_x) % W;    // resto x píxel para ajustar un bloque
    int y_remainder = (pac_y - game->geo_y) % W;    // resto y píxel para ajustar un bloque
    Dir next_dir = get_next_dir();

    /* Cuando pacman encaja completamente en un bloque,
     * decidir si cambiar de dirección. */
    if (x_remainder == 0) {
        if (y_remainder == 0) {
            // Cuando pacman encaja completamente en un bloque,
            // prueba si este bloque es una pelota para comer.
            // Si se comen todas las bolas, gana.
            eat_ball(__y, __x);

            // actualizar la coordenada de pacman en el mapa
            _x = __x;
            _y = __y;

            if (game->ball_num == 0) {
                game->stat = Game::Win;
                return;
            }
        }

        switch (next_dir) {
        case Stop:
            dir = next_dir;
            break;
        case Up:        // probar si puede girar hacia arriba
            if (overlapable(_y - 1, _x)) {
                dir = next_dir;
            }
            break;
        case Down:      // probar si puede girar hacia abajo
            if (overlapable(_y + 1, _x)) {
                dir = next_dir;
            }
            break;
        case Left:      // prueba si puede girar a la izquierda
            if (y_remainder == 0 && overlapable(_y, _x - 1)) {
                dir = next_dir;
            }
            break;
        case Right:      // prueba si puede girar a la derecha
            if (y_remainder == 0 && overlapable(_y, _x + 1)) {
                dir = next_dir;
            }
            break;
        }
    } else if (y_remainder == 0) {
        switch (next_dir) {
        case Stop:
            dir = next_dir;
            break;
        case Left:      // prueba si puede girar a la izquierda
            if (overlapable(_y, _x - 1)) {
                dir = next_dir;
            }
            break;
        case Right:     // prueba si puede girar a la derecha
            if (overlapable(_y, _x + 1)) {
                dir = next_dir;
            }
            break;
        default:                    // decidir cuando x_resto = 0
            break;
        }
    }

    /* Pacman sigue moviéndose o se detiene cuando golpea la pared */
    switch (dir) {
    case Stop:
        break;
    case Up:
        if (y_remainder == 0 && !overlapable(_y - 1, _x)) {
            dir = Stop;
            next_dir = Stop;
        } else {
            moveup();
        }
        break;
    case Down:
        if (y_remainder == 0 && !overlapable(_y + 1, _x)) {
            dir = Stop;
            next_dir = Stop;
        } else {
            movedown();
        }
        break;
    case Left:
        if (x_remainder == 0 && !overlapable(_y, _x - 1)) {
            dir = Stop;
            next_dir = Stop;
        } else {
            moveleft();
        }
        break;
    case Right:
        if (x_remainder == 0 && !overlapable(_y, _x + 1)) {
            dir = Stop;
            next_dir = Stop;
        } else {
            moveright();
        }
        break;
    }
}
