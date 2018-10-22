// Copyright (c) 2018 Luma Pictures . All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. All advertising materials mentioning features or use of this software
// must display the following acknowledgement:
// This product includes software developed by the the organization.
//
// 4. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include "pxr/usd/usdAi/utils.h"

#include <array>

PXR_NAMESPACE_OPEN_SCOPE

UsdStageRefPtr UsdAiGetArnoldShaderDesc(const std::string& additionalFlags) {
    UsdStageRefPtr ret = nullptr;
    std::stringstream command;
    command << "usdAiShaderInfo --cout";
    if (!additionalFlags.empty()) { command << " " << additionalFlags; }
    FILE* pipe = popen(command.str().c_str(), "r");
    if (pipe != nullptr) {
        std::stringstream result;
        std::array<char, 4096> buffer = {0};
        while (!feof(pipe)) {
            if (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
                result << buffer.data();
            }
        }
        pclose(pipe);
        ret = UsdStage::CreateInMemory(".usda");
        ret->GetRootLayer()->ImportFromString(result.str());
    }
    return ret;
}

PXR_NAMESPACE_CLOSE_SCOPE