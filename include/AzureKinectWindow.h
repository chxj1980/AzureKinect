#pragma once
/**
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

#include "AzureKinect.h"
#include "ui_AzureKinect.h"

#include <QIntValidator>
#include <QtWidgets/QMainWindow>
#include <array>
#include <vector>

namespace Ak {
class AzureKinectWindow final : public QMainWindow
{
    Q_OBJECT

    friend class KinectWidget;

public:
    explicit AzureKinectWindow(QWidget* parent = Q_NULLPTR) noexcept;

public slots:

    /** Slot used to receive request to start recording camera data */
    void startSlot() noexcept;

    /** Slot used to receive request to exit the program */
    void exitSlot() noexcept;

    /** Slot used to pass error messages between threads/objects (calls exitSlot when done) */
    void errorSlot(const QString& message) noexcept;

    /** Slot used to notify program that camera is ready to start capture */
    void readySlot() const noexcept;

    /** Slot used to select display of depth image */
    void depthImageSlot() noexcept;

    /** Slot used to select display of colour image */
    void colourImageSlot() noexcept;

    /** Slot used to select display of IR image */
    void irImageSlot() noexcept;

    /** Slot used to select display of body shadow */
    void bodyShadowSlot() noexcept;

    /** Slot used to select display of body skeleton */
    void bodySkeletonSlot() noexcept;

private:
    Ui::AzureKinectClass m_ui;
    QValidator* m_validatorPID = nullptr;
    std::array<std::vector<uint8_t>, 15> m_imageBuffer;
    uint32_t m_bufferIndex = 0;
    bool m_depthImage = true;
    bool m_colourImage = false;
    bool m_irImage = false;
    bool m_bodyShadowImage = true;
    bool m_bodySkeletonImage = true;
    bool m_started = false;
    AzureKinect m_kinect;

    /**
     * Override used to capture and handle close events.
     * @param [in,out] event If non-null, the event.
     */
    void closeEvent(QCloseEvent* event) noexcept override;

    /**
     * Callback used by the camera thread to notify of errors.
     * @note This provide thread safe, asynchronous error handling.
     * @param message The error message.
     */
    void errorCallback(const std::string& message) const noexcept;

    /**
     * Callback used by the camera thread to notify when it ready to begin recording
     * @note This provide thread safe, asynchronous handling.
     */
    void readyCallback() const noexcept;

    /**
     * Callback used by the camera thread when new image information is available.
     * @param depthImage  The depth image data.
     * @param colourImage The colour image data.
     * @param irImage     The IR image data.
     */
    void imageCallback(const AzureKinect::KinectImage& depthImage, const AzureKinect::KinectImage& colourImage,
        const AzureKinect::KinectImage& irImage) noexcept;

signals:
    /**
     * Signal used to pass asynchronous thread safe error messages.
     * @note This is required by @errorCallback.
     * @param message The message.
     */
    void errorSignal(const QString& message) const;

    /**
     * Signal used to pass asynchronous thread safe ready state.
     * @note This is required by @readyCallback.
     */
    void readySignal() const;

    /**
     * Signal used to pass asynchronous thread safe image data.
     * @note This is required by @imageCallback.
     * @param depthImage  The depth image data.
     * @param colourImage The colour image data.
     * @param irImage     The IR image data.
     */
    void imageSignal(AzureKinect::KinectImage depthImage, AzureKinect::KinectImage colourImage,
        AzureKinect::KinectImage irImage) const;
};
} // namespace Ak
