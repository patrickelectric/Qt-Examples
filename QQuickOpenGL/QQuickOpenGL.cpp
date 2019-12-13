#include "QQuickOpenGL.h"

#include <QQuickWindow>
#include <QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QSGSimpleRectNode>

#include <math.h>

#include <QDebug>

QQuickOpenGL::QQuickOpenGL(QQuickItem *parent)
    : QQuickItem(parent)
    , _shaderProgram(nullptr)
    , _shift(0)
    , _window(nullptr)
{
    // We are going to fill our own content with OpenGL
    setFlag(QQuickItem::ItemHasContents);

    connect(this, &QQuickItem::windowChanged, this, &QQuickOpenGL::handleWindowChanged);
    connect(this, &QQuickOpenGL::shiftChanged, this, [this]{ if (_window) _window->update(); });
}

void QQuickOpenGL::handleWindowChanged(QQuickWindow* window)
{
    if (!window) {
        return;
    }

    _window = window;
    window->setClearBeforeRendering(false);

    connect(window, &QQuickWindow::beforeRendering, this, &QQuickOpenGL::paint, Qt::DirectConnection);
}

void QQuickOpenGL::paint()
{
    // Check if we are rendering with OpenGL
    QSGRendererInterface *rif = window()->rendererInterface();
    Q_ASSERT(rif->graphicsApi() == QSGRendererInterface::OpenGL);

    initializeOpenGLFunctions();

    if(!_shaderProgram) {
        _shaderProgram.reset(new QOpenGLShaderProgram());
        _shaderProgram->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
        R"(
            attribute vec4 vertices;
            varying vec2 coords;
            void main() {
                gl_Position = vertices;
                coords = vertices.xy;
            }
        )");

        _shaderProgram->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
        R"(
            varying vec2 coords;
            uniform float shift;

            void main() {
                gl_FragColor = vec4(shift, coords.x, coords.y, 1.0);
            }
        )");

        _shaderProgram->bindAttributeLocation("vertices", 0);
        _shaderProgram->link();
    }

    float values[] = {
        1, 1,
        -1, 1,
        1, -1,
        -1, -1
    };

    // Bind with the actual OpenGL thread
    _shaderProgram->bind();
    _shaderProgram->enableAttributeArray(0);
    _shaderProgram->setAttributeArray(0, GL_FLOAT, values, 2);
    _shaderProgram->setUniformValue("shift", static_cast<float>((_shift%100)/100.0f));

    // We need to fix the opengl origin with our actual item position
    auto point = parentItem()->mapFromItem(this, {0, 0});
    point += QPointF{0, static_cast<float>(parentItem()->height() - height())};
    glViewport(point.x(), point.y(), width(), height());
    glDisable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    _shaderProgram->disableAttributeArray(0);
    _shaderProgram->release();

    _window->resetOpenGLState();
}
