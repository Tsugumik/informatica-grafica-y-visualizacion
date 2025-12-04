#include "Material.h"
#include <cstring>

Material::Material() {
    GLfloat default_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat default_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat default_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
    memcpy(ambient, default_ambient, sizeof(default_ambient));
    memcpy(diffuse, default_diffuse, sizeof(default_diffuse));
    memcpy(specular, default_specular, sizeof(default_specular));
    shininess = 0.0f;
}

Material::Material(const GLfloat* amb, const GLfloat* diff, const GLfloat* spec, GLfloat shin) {
    memcpy(ambient, amb, 4 * sizeof(GLfloat));
    memcpy(diffuse, diff, 4 * sizeof(GLfloat));
    memcpy(specular, spec, 4 * sizeof(GLfloat));
    shininess = shin;
}

void Material::apply() const {
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
