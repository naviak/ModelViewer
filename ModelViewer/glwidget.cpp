#include "glwidget.h"

GLWidget::GLWidget(QWidget* parent) :
    QOpenGLWidget(parent),
    m_model(nullptr),
    m_timer(0)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

GLWidget::~GLWidget() {
    makeCurrent();
    delete m_model;
    delete road;
	for(auto &a:m_program){ delete a; }
    doneCurrent();
}

QOpenGLShaderProgram* GLWidget::newShader()
{
    auto shader = new QOpenGLShaderProgram(this);
    assert(shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "shadernew.vert"));
    assert(shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "shadernew.frag"));
    assert(shader->link());
    assert(shader->bind());
    shader->setUniformValue("vLight", QVector3D(2, 2, 2));
    shader->setUniformValue("ambientLight", QVector4D(0.3f, 0.3f, 0.3f, 1.f));
    return shader;
}

QOpenGLShaderProgram* GLWidget::simpleShader()
{
    auto shader = new QOpenGLShaderProgram(this);
    assert(shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaderv.vert"));
    assert(shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "shader.frag"));
    assert(shader->link());
    assert(shader->bind());
    shader->setUniformValue("vLight", QVector3D(2, 2, 2));
    shader->setUniformValue("ambientLight", QVector4D(0.3f, 0.3f, 0.3f, 1.f));
    return shader;
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    setFocusPolicy(Qt::StrongFocus);
    m_program.push_back(simpleShader());
    m_program.push_back(newShader());
    glUseProgram(m_program[shader_num]->programId());
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start(1000/60.0f);
    loadRoad();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    int a = width();
    int b = height();
}


void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program[shader_num]->bind();
    m_program[shader_num]->setUniformValue("model", m_modelMat);
    m_program[shader_num]->setUniformValue("view", _camera.getViewMatrix());
    m_program[shader_num]->setUniformValue("projection", m_projectionMat);
    if (shader_num)
    {
        m_program[shader_num]->setUniformValue("rt_w", static_cast<float>(width()));
        m_program[shader_num]->setUniformValue("rt_h", static_cast<float>(height()));
    }
    makeCurrent();
    road->draw(this, m_program[shader_num]);
    doneCurrent();
    if (m_model != nullptr) {
        makeCurrent();
        m_model->draw(this,m_program[shader_num]);
        doneCurrent();
    }
    moveCamera();
    Input::reset();
    MouseInput::reset();
    update();
}

void GLWidget::loadModel(QString filename) {
    _filename = filename;
	if(m_model != nullptr ) delete m_model;
    makeCurrent();
    m_model = new Model(filename, this,m_program[shader_num],!shader_num);
    doneCurrent();
}

void GLWidget::loadRoad()
{
    road = new Model("C:/Users/Max Dudar/source/repos/ModelViewer/ModelViewer/Meshes/gr3.obj", this, m_program[shader_num], !shader_num);
}

void GLWidget::setNewShader(int newshader)
{
	
    shader_num = newshader/2;
    glUseProgram(m_program[shader_num]->programId());
	if(!_filename.isEmpty()) loadModel(_filename);
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
