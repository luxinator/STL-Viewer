//
// Created by lucas on 4/21/16.
//

#ifndef STLVIEWER_GLERROR_H
#define STLVIEWER_GLERROR_H



void _check_gl_error(const char *file, int line);

///
/// Usage
/// [... some opengl calls]
/// glCheckError();
///
#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

#endif //STLVIEWER_GLERROR_H
