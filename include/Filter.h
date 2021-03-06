﻿#pragma once
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

#include <functional>
#include <memory>
#include <string>

extern "C" {
#include <libavutil/pixfmt.h>
#include <libavutil/rational.h>
}

struct AVFilterGraph;
struct AVFilterContext;

namespace Ak {
class FramePtr;

class Filter
{
    friend class Encoder;

    class FilterGraphPtr
    {
    public:
        FilterGraphPtr() = default;

        explicit FilterGraphPtr(AVFilterGraph* filterGraph) noexcept;

        [[nodiscard]] AVFilterGraph* get() const noexcept;

        AVFilterGraph* operator->() const noexcept;

        std::shared_ptr<AVFilterGraph> m_filterGraph = nullptr;
    };

public:
    using errorCallback = std::function<void(const std::string&)>;

    Filter() = default;

    ~Filter() = default;

    Filter(const Filter& other) = delete;

    Filter(Filter&& other) noexcept = default;

    Filter& operator=(const Filter& other) = delete;

    Filter& operator=(Filter&& other) noexcept = default;

    /**
     * Initializes the filter.
     * @param width      The input frame width.
     * @param height     The input frame height.
     * @param fps        The input frame FPS.
     * @param format     The input frame pixel format to use.
     * @param scale      The scale that needs to be applied to input pixels.
     * @param numThreads Number of threads.
     * @param error      (Optional) The callback used to signal errors.
     * @returns True if it succeeds, false if it fails.
     */
    bool init(uint32_t width, uint32_t height, AVRational fps, int32_t format, float scale, uint32_t numThreads,
        errorCallback error = nullptr) noexcept;

    /**
     * Sends a frame to be filtered
     * @param [in,out] frame The input frame.
     * @returns True if it succeeds, false if it fails.
     */
    bool sendFrame(FramePtr& frame) const noexcept;

    /**
     * Receive frame from filter graph
     * @param [in,out] frame The frame.
     * @returns True if it succeeds, false if it fails.
     */
    bool receiveFrame(FramePtr& frame) const noexcept;

    /**
     * Gets the width of output frames.
     * @returns The width.
     */
    [[nodiscard]] uint32_t getWidth() const noexcept;

    /**
     * Gets the height of output frames.
     * @returns The height.
     */
    [[nodiscard]] uint32_t getHeight() const noexcept;

    /**
     * Gets pixel format of output frames.
     * @returns The pixel format.
     */
    [[nodiscard]] AVPixelFormat getPixelFormat() const noexcept;

    /**
     * Gets the frame rate (fps) of the output frames.
     * @note This will not be fully accurate when dealing with VFR video streams.
     * @returns The frame rate in frames per second.
     */
    [[nodiscard]] AVRational getFrameRate() const noexcept;

private:
    FilterGraphPtr m_filterGraph;        /**< The filter graph. */
    AVFilterContext* m_source = nullptr; /**< The input for the filter graph. */
    AVFilterContext* m_sink = nullptr;   /**< The output of the filter graph.*/
    errorCallback m_errorCallback = nullptr;
};
} // namespace Ak
