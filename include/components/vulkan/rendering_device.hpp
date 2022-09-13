#ifndef RENDERING_DEVICE
#define RENDERING_DEVICE

#include <foundation.hpp>
#include <glm/glm.hpp>

/*****************/
/**** GENERIC ****/
/*****************/

enum CompareOperator
{
    COMPARE_OP_NEVER,
    COMPARE_OP_LESS,
    COMPARE_OP_EQUAL,
    COMPARE_OP_LESS_OR_EQUAL,
    COMPARE_OP_GREATER,
    COMPARE_OP_NOT_EQUAL,
    COMPARE_OP_GREATER_OR_EQUAL,
    COMPARE_OP_ALWAYS,
    COMPARE_OP_MAX
};

struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;

    local vk::VertexInputBindingDescription getBindingDescription()
    {
        vk::VertexInputBindingDescription bindingDescription;

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = vk::VertexInputRate::eVertex;

        return bindingDescription;
    }

    local std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescription()
    {
        std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions;
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);
        attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;

        return attributeDescriptions;
    }
};

/******************/
/**** TEXTURES ****/
/******************/

enum Samples
{
    SAMPLES_1,
    SAMPLES_2,
    SAMPLES_4,
    SAMPLES_8,
    SAMPLES_16,
    SAMPLES_32,
    SAMPLES_64,
    SAMPLES_MAX
};

/***************************/
/**** GRAPHICS PIPELINE ****/
/***************************/

enum PipelinePrimitive
{
    PIPELINE_PRIMITIVE_POINTS,
    PIPELINE_PRIMITIVE_LINES,
    PIPELINE_PRIMITIVE_LINES_WITH_ADJACENCY,
    PIPELINE_PRIMITIVE_LINESTRIPS,
    PIPELINE_PRIMITIVE_LINESTRIPS_WITH_ADJACENCY,
    PIPELINE_PRIMITIVE_TRIANGLES,
    PIPELINE_PRIMITIVE_TRIANGLES_WITH_ADJACENCY,
    PIPELINE_PRIMITIVE_TRIANGLE_STRIPS,
    PIPELINE_PRIMITIVE_TRIANGLE_STRIPS_WITH_AJACENCY,
    PIPELINE_PRIMITIVE_TRIANGLE_STRIPS_WITH_RESTART_INDEX,
    PIPELINE_PRIMITIVE_TESSELATION_PATCH,
    PIPELINE_PRIMITIVE_MAX //* Not an operator - Represents max number of available operators
};

enum PipelineCullMode
{
    CULL_DISABLED,
    CULL_FRONT,
    CULL_BACK
};

enum PipelineFrontFace
{
    FRONT_FACE_CLOCKWISE,
    FRONT_FACE_COUNTER_CLOCKWISE
};

enum StencilOperation
{
    STENCIL_OP_KEEP,
    STENCIL_OP_ZERO,
    STENCIL_OP_REPLACE,
    STENCIL_OP_INCREMENT_AND_CLAMP,
    STENCIL_OP_DECREMENT_AND_CLAMP,
    STENCIL_OP_INVERT,
    STENCIL_OP_INCREMENT_AND_WRAP,
    STENCIL_OP_DECREMENT_AND_WRAP,
    STENCIL_OP_MAX //* Not an operator - Represents max number of available operators
};

struct PipelineRasterizationState
{
    bool enableDepthClamp;
    bool discardPrimitives;
    bool enableDepthBias;
    bool wireframeMode;
    PipelineCullMode cullMode;
    PipelineFrontFace frontFace;
    float depthBiasConstantFactor;
    float depthBiasClamp;
    float depthBiasSlopeFactor;
    float lineWidth;
    u32 patchControlPoints;

    PipelineRasterizationState()
    {
        enableDepthClamp = false;
        enableDepthBias = false;
        discardPrimitives = false;
        wireframeMode = false;
        cullMode = CULL_BACK;
        frontFace = FRONT_FACE_CLOCKWISE;
        depthBiasConstantFactor = 0;
        depthBiasClamp = 0;
        depthBiasSlopeFactor = 0;
        lineWidth = 1.0f;
        patchControlPoints = 1;
    }
};

struct PipelineMultisampleState
{
    Samples sampleCount;
    bool enableSampleShading;
    bool enableAlphaToCoverage;
    bool enableAlphaToOne;
    std::vector<u32> sampleMask;
    float minSampleShading;

    PipelineMultisampleState()
    {
        sampleCount = SAMPLES_1;
        enableSampleShading = false;
        enableAlphaToCoverage = false;
        enableAlphaToOne = false;
        minSampleShading = 0.2f;
    }
};

struct PipelineDepthStencilState
{
    bool enableDepthTest;
    bool enableDepthWrite;
    bool enableDepthRange;
    bool enableStencil;
    float depthRangeMin;
    float depthRangeMax;
    CompareOperator depthCompareOperator;

    struct StencilOperationState {
        StencilOperation pass;
        StencilOperation fail;
        StencilOperation depthFail;
        CompareOperator compare;
        u32 compareMask;
        u32 writeMask;
        u32 reference;

        StencilOperationState(){
            pass = STENCIL_OP_ZERO;
            fail = STENCIL_OP_ZERO;
            depthFail = STENCIL_OP_ZERO;
            compare = COMPARE_OP_ALWAYS;
            compareMask = 0;
            writeMask = 0;
            reference = 0;
        }
    };

    StencilOperationState frontOp;
    StencilOperationState backOp;

    PipelineDepthStencilState() {
        enableDepthTest = true;
        enableDepthWrite = true;
        enableDepthRange = false;
        enableStencil = false;
        depthCompareOperator = COMPARE_OP_LESS;
        depthRangeMin = 0.0f;
        depthRangeMax = 1.0f;
    }
};

#endif