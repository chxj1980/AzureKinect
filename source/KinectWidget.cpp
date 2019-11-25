﻿/**
 * Copyright Matthew Oliver
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "KinectWidget.h"

#include <QOpenGLContext>

namespace Ak {
KinectWidget::KinectWidget(QWidget* parent) noexcept
    : QOpenGLWidget(parent)
{}

KinectWidget::~KinectWidget() noexcept
{
    cleanup();
}

void KinectWidget::initializeGL() noexcept
{
    initializeOpenGLFunctions();
    // Connect cleanup handler
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &KinectWidget::cleanup);
}

void KinectWidget::resizeGL(const int width, const int height) noexcept
{
    const int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void KinectWidget::paintGL() noexcept {}

void KinectWidget::cleanup() noexcept
{
    // Make sure the context is current and then destroy all underlying resources.
    makeCurrent();

    // TODO: free resources here

    doneCurrent();
}
} // namespace Ak
