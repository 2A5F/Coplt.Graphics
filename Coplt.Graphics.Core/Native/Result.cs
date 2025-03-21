﻿using System.Diagnostics;

namespace Coplt.Graphics.Native;

public partial struct FResult
{
    [StackTraceHidden]
    public void TryThrow()
    {
        if (code == FResultCode.Success) return;
        var msg = this.msg.GetString();
        throw new NativeException(code, msg);
    }
}
