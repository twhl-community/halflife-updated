mod vector;

use std::ffi::c_void;
use std::intrinsics::sqrtf32;

pub fn add(left: usize, right: usize) -> usize {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }
}



struct CBasePlayer {}

impl CBasePlayer {
    #[no_mangle]
    pub fn Jump() {
        Vector
        vecWallCheckDir; // direction we're tracing a line to find a wall when walljumping
        Vector
        vecAdjustedVelocity;
        Vector
        vecSpot;
        TraceResult
        tr;

        if (FBitSet(pev->flags, FL_WATERJUMP))
        return;

        if (pev -> waterlevel >= 2)
        {
            return;
        }

        // jump velocity is sqrt( height * gravity * 2)

        // If this isn't the first frame pressing the jump button, break out.
        if (!FBitSet(m_afButtonPressed, IN_JUMP))
        return; // don't pogo stick

        if ((pev -> flags & FL_ONGROUND) == 0 || !pev -> groundentity)
        {
            return;
        }

        // many features in this function use v_forward, so makevectors now.
        UTIL_MakeVectors(pev->angles);

        // ClearBits(pev->flags, FL_ONGROUND);		// don't stairwalk

        SetAnimation(PLAYER_JUMP);

        if (m_fLongJump &&
            (pev -> button & IN_DUCK) != 0 &&
            (pev -> flDuckTime > 0) &&
            pev -> velocity.Length() > 50)
        {
            SetAnimation(PLAYER_SUPERJUMP);
        }

        // If you're standing on a conveyor, add it's velocity to yours (for momentum)
        entvars_t * pevGround = VARS(pev->groundentity);
        if (pevGround && (pevGround -> flags & FL_CONVEYOR) != 0)
        {
            pev -> velocity = pev -> velocity + pev -> basevelocity;
        }
    }
}
