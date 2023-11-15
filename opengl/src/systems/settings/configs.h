#include <GL/glew.h>


struct PolygonModeConfig
{
    GLenum Face = GL_FRONT_AND_BACK;
    GLenum Mode = GL_FILL;

    bool operator!=(const PolygonModeConfig& other) const
    {
        return Face != other.Face || Mode != other.Mode;
    }
};

struct DepthConfig
{
    bool Enabled = true;
    GLenum Function = GL_LESS;

    bool operator!=(const DepthConfig& other) const
    {
        return Enabled != other.Enabled ||
            Function != other.Function;
    }
};

struct PolygonSmoothingConfig
{
    bool Enabled = true;
    GLenum Hint = GL_FASTEST;

    bool operator!=(const PolygonSmoothingConfig& other) const
    {
        return Enabled != other.Enabled ||
            Hint != other.Hint;
    }
};



struct BlendingConfig
{
    bool Enabled = true;
    GLenum SourceFactor = GL_SRC_ALPHA;
    GLenum DestinationFactor = GL_ONE_MINUS_SRC_ALPHA;
    GLenum Equation = GL_FUNC_ADD;

    bool operator!=(const BlendingConfig& other) const
    {
        return Enabled != other.Enabled || 
        SourceFactor != other.SourceFactor || 
        DestinationFactor != other.DestinationFactor || 
        Equation != other.Equation;
    }
};

struct CullingConfig
{
    bool Enabled = true;
    GLenum Face = GL_BACK;
    GLenum WindingOrder = GL_CCW;

    bool operator!=(const CullingConfig& other) const
    {
        return Enabled != other.Enabled || 
        Face != other.Face || 
        WindingOrder != other.WindingOrder;
    }
};


struct RenderConfig
{
    float TimeScale = 1.0f;
    PolygonModeConfig PolygonMode;
    PolygonSmoothingConfig PolygonSmoothing;
    DepthConfig Depth;
    BlendingConfig Blending;
    CullingConfig Culling;

    bool operator!=(const RenderConfig& other) const
    {
        return PolygonMode != other.PolygonMode || 
        PolygonSmoothing != other.PolygonSmoothing || 
        Depth != other.Depth || 
        Blending != other.Blending || 
        Culling != other.Culling;
    }
};