#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions_3_3_Core>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QLabel>
#include "model.h"
#include "camera.h"
#include "FpsMonitor.h"
#include "Input.h"
#include "MouseInput.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    GLWidget(QWidget* parent);
    ~GLWidget() override;
	QOpenGLShaderProgram* newShader();
	QOpenGLShaderProgram* simpleShader();
    void loadModel(QString filename);
	void loadRoad();
public slots:
    void setPolygonMode(int wireframe);
	void setNewShader(int newshader);

protected:
    void initializeGL() override;
    void paintGL() override;
	void moveCamera() {
		std::map<int, QVector3D> controls = {
			{Qt::Key_W, {0,0,1}},
			{Qt::Key_S, {0,0,-1}},
			{Qt::Key_A, {-1,0,0}},
			{Qt::Key_D, {1,0,0}},
			{Qt::Key_E, {0,1,0}},
			{Qt::Key_Q, {0,-1,0}} };

		for (auto& [key, dir] : controls)
		{
			if (Input::keyPressed(key))
			{
				_camera.translate(dir * 0.02f);
			}
		}
		if (Input::keyPressed(Qt::LeftButton))
			_camera.look(MouseInput::delta().x() * 0.06f, MouseInput::delta().y() * 0.06f);
	}
	void wheelEvent(QWheelEvent* event) override
	{
		auto dr = 0.f;
		const auto offset = 1.f;
		if (event->angleDelta().y() > 0) {
			dr = offset * _camera.front.z() / abs(_camera.front.z());
		}
		else if (event->angleDelta().y() < 0) {
			dr = -offset * _camera.front.z() / abs(_camera.front.z());
		}
		_camera.position = { _camera.position.x() + dr * _camera.front.x(),
						_camera.position.y() + dr * _camera.front.y(),
						_camera.position.z() + dr * _camera.front.z() };
	}
	void keyPressEvent(QKeyEvent* e) override
	{
		Input::pressKey(e->nativeVirtualKey());
		QOpenGLWidget::keyPressEvent(e);
	}

	void keyReleaseEvent(QKeyEvent* e) override
	{
		Input::releaseKey(e->nativeVirtualKey());
		QOpenGLWidget::keyReleaseEvent(e);
	}

	void mouseMoveEvent(QMouseEvent* e) override
	{
		MouseInput::mouseCallback(e->pos());
		QOpenGLWidget::mouseMoveEvent(e);
	}

	void mousePressEvent(QMouseEvent* e) override
	{
		Input::pressKey(e->button());
		QOpenGLWidget::mousePressEvent(e);
	}

	void mouseReleaseEvent(QMouseEvent* e) override
	{
		Input::releaseKey(e->button());
		QOpenGLWidget::mouseReleaseEvent(e);
	}
	void resizeGL(int w, int h) override
	{
		m_projectionMat.setToIdentity();
		m_projectionMat.perspective(60.0f, GLfloat(w) / h, 0.01f, 100.0f);
	}

private:
    Model* m_model = nullptr;
    QMatrix4x4 m_modelMat;
    QMatrix4x4 m_projectionMat;
	Model* road = nullptr;
    Camera _camera;
    QTimer* m_timer;
    QPoint m_lastMousePos;
	int shader_num = 0;
    QVector<QOpenGLShaderProgram*> m_program;
	QOpenGLShaderProgram* nm_program;
	QString _filename;
	FpsMonitor fps;
	QLabel* labfps;
	QPalette sample_palette;
};

#endif // GLWIDGET_H
