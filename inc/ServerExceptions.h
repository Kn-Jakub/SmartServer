/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ServerExceptions.h
 * Author: JosoP
 *
 * Created on Utorok, 2017, decembra 19, 10:32
 */

#ifndef SERVEREXCEPTIONS_H
#define SERVEREXCEPTIONS_H
#include "../lbr_c++/definitions.h"
#ifdef __cplusplus
extern "C" {
#endif

#define EX_PORT_WRONG_NUMBER    1
#define EX_SOCK_CREATING        2
#define EX_BIND                 3
#define EX_ACCEPT               4
#define EX_SOCK_ACCEPT          5

#ifdef __cplusplus
}
#endif

#endif /* SERVEREXCEPTIONS_H */

