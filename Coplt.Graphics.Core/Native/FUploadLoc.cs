using Coplt.Graphics.Core;

namespace Coplt.Graphics.Native;

public partial struct FUploadLoc
{
    public FUploadLoc(in UploadLoc loc)
    {
        Index = (uint)loc.Index;
    }
    
    public FUploadLoc(in UploadLoc2 loc)
    {
        Index = (uint)loc.Index;
    }

    public static implicit operator FUploadLoc(in UploadLoc loc) => new(loc);
    public static implicit operator FUploadLoc(in UploadLoc2 loc) => new(loc);
}
