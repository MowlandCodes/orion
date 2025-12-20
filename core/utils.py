import numpy as np
import math
from config import ROBOT_RADIUS

def inverse_kinematics(vx, vy, omega):
    """
    change the robot speed target (global) into individual motor velocity
    Input: vx (m/s), vy (m/s), omega (rad/s)
    Output: v1, v2, v3 (m/s linear speed roda)
    """
    L = ROBOT_RADIUS

    # Omni 3-Wheel Kinematic Matrix (Assuming angles of 150, 270, 30 degrees)
    # Wheel 1: Front Left (150 degrees) -> sin(150)=0.5, cos(150)=-0.866
    # Wheel 2: Front Right (30 degrees) -> sin(30)=0.5, cos(30)=0.866
    # Wheel 3: Rear (270 degrees) -> sin(270)=-1, cos(270)=0

    # Core formula: v_wheel = -sin(theta) * vx + cos(theta) * vy + L * omega
    # Rotation component (omega) is multiplied with L.
    
    v1 = -0.5 * vx - 0.866 * vy + L * omega
    v2 = -0.5 * vx + 0.866 * vy + L * omega
    v3 = 1.0 * vx + 0.000 * vy + L * omega

    return v1, v2, v3
