#include "glwidget.h"

GLWidget::GLWidget(QWidget* parent) :
    QOpenGLWidget(parent),
    m_model(nullptr),
    m_timer(0),
    m_program(0)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

GLWidget::~GLWidget() {
    makeCurrent();
    delete m_model;
    delete m_program;
    doneCurrent();
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    setFocusPolicy(Qt::StrongFocus);
    m_program = new QOpenGLShaderProgram(this);
    assert(m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaderv.vert"));
    assert(m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shader.frag"));
    assert(m_program->link());
    assert(m_program->bind());
    m_program->setUniformValue("vLight", QVector3D(2, 2, 2));
    glUseProgram(m_program->programId());
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start(1000/60.0f);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
}


void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();
    m_program->setUniformValue("model", m_modelMat);
    m_program->setUniformValue("view", _camera.getViewMatrix());
    m_program->setUniformValue("projection", m_projectionMat);

    if (m_model != nullptr) {
        makeCurrent();
        m_model->draw(this,m_program);
        doneCurrent();
    }
    moveCamera();
    Input::reset();
    MouseInput::reset();
    update();
}

void GLWidget::loadModel(QString filename) {
    makeCurrent();
    m_model = new Model(filename, this,m_program);
    doneCurrent();
}


void GLWidget::setPolygonMode(int wireframe) {
    makeCurrent();
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    doneCurrent();
}
