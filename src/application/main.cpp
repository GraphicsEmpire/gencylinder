#include <iostream>
#include "opengl/glselect.h"
#include <GLFW/glfw3.h>

//curve
#include "catmullromcurverender.h"
#include "catmullromcurve.h"
#include "linalg/orthographiccamera.h"

using namespace std;

#define APP_NAME "gencylinder"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 512

//vars
std::shared_ptr<nb::render::CatmullRomCurve> gCurveData;
std::shared_ptr<nb::linalg::OrthographicCamera> gOrthoCamera;

void key_handler(GLFWwindow *w, int key, int scancode, int action, int mods) {
    if (w == NULL)
        return;

    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(w, 1);
    }
}

void mouse_handler(GLFWwindow *w, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        gCurveData->Clear();
        cout << "CtrlPoints# " << gCurveData->CountCtrlPoints() << endl;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        double xpos, ypos;
        glfwGetCursorPos(w, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(w, &width, &height);

        double ndc_x = xpos / (double) width;
        double ndc_y = 1.0 - (ypos / (double) height);
        cout << "MousePos (ndc) = [" << ndc_x << " ," << ndc_y << "]" << endl;

        gCurveData->AddCtrlPoint(nb::linalg::Vec3<float>(ndc_x, ndc_y, 0.0));
        cout << "CtrlPoints# " << gCurveData->CountCtrlPoints() << endl;
    }
}

int main(int argc, char *argv[]) {
    cout << "INFO: gencylinder starts" << endl;

    if (glfwInit() != GLFW_TRUE) {
        cerr << "ERROR: Unable to init glfw" << endl;
        exit(1);
    }

    GLFWwindow *w = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME, NULL, NULL);
    glfwSetKeyCallback(w, key_handler);
    glfwSetMouseButtonCallback(w, mouse_handler);

    //init glbinding
    glbinding::Binding::initialize();

    //create the curve
    gCurveData.reset(new nb::render::CatmullRomCurve());
    std::shared_ptr<nb::render::CatmullRomCurveRender> curverender(new nb::render::CatmullRomCurveRender(gCurveData));

    //camera
    gOrthoCamera.reset(new nb::linalg::OrthographicCamera());
    gOrthoCamera->SetPlanes(0.0f, 1.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(w)) {

        //render
        curverender->Render(gOrthoCamera->GetViewMatrix(0), gOrthoCamera->GetProjectionMatrix(0));

        //swap
        glfwPollEvents();
        glfwSwapBuffers(w);
    }


    glfwTerminate();


    return 0;
}
