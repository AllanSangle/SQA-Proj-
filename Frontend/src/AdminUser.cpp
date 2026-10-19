#include "AdminUser.h"

bool AdminUser::canBuy() const {
    return true;
}

bool AdminUser::canSell() const {
    return true;
}

bool AdminUser::isAdmin() const {
    return true;
}

std::string AdminUser::getUserType() const {
    return "AA";
}
