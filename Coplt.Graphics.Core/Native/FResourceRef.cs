namespace Coplt.Graphics.Native;

public partial struct FResourceRef
    : IEquatable<FResourceRef>
{
    public FResourceRef(uint index)
    {
        ResourceIndex = index;
    }
    public FResourceRef(int index)
    {
        ResourceIndex = (uint)index;
    }

    #region Equals

    public bool Equals(FResourceRef other) => ResourceIndex == other.ResourceIndex;
    public override bool Equals(object? obj) => obj is FResourceRef other && Equals(other);
    public override int GetHashCode() => (int)ResourceIndex;
    public static bool operator ==(FResourceRef left, FResourceRef right) => left.Equals(right);
    public static bool operator !=(FResourceRef left, FResourceRef right) => !left.Equals(right);

    #endregion
}
