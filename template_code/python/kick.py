def kick(motion_proxy, side=0):
    '''
    Perform the DNT kick.
    '''
    motion_proxy.setFallManagerEnabled(False)
    if side == 1:
        # Kick towards left with right leg
        names = ['RShoulderRoll', 'RShoulderPitch', 'LShoulderRoll', 'LShoulderPitch', 'RHipRoll',
                 'RHipPitch', 'RKneePitch', 'RAnklePitch', 'RAnkleRoll', 'LHipRoll', 'LHipPitch',
                 'LKneePitch', 'LAnklePitch', 'LAnkleRoll']
        angles = [[-0.3], [0.4], [1.0], [1.0], [0.0], [-0.4, -0.2], [0.95, 1.5], [-0.55, -1],
                  [0.2], [0.0], [-0.4], [0.95], [-0.55], [0.2]]
        times =  [[ 0.5], [0.5], [0.5], [0.5], [0.5], [ 0.4,  0.8], [ 0.4, 0.8],  [0.4, 0.8],
                  [0.4], [0.5], [ 0.4], [ 0.4], [ 0.4],  [0.4]]

        motion_proxy.angleInterpolation(names, angles, times, True)

        motion_proxy.angleInterpolation(['RShoulderPitch', 'RHipPitch', 'RKneePitch',
                                         'RAnklePitch', 'LShoulderPitch'],
                                         [1.0, -0.7, 1.05, -0.5, 0.2],
                                         [[0.1], [0.1], [0.1], [0.1], [0.1]], True)

        motion_proxy.angleInterpolation(['RHipPitch', 'RKneePitch', 'RAnklePitch'],
                                        [-0.5, 1.1, -0.65],
                                        [[0.25], [0.25], [0.25]], True)
    else:
        # Kick towards front, left leg
        names = ['LShoulderRoll', 'LShoulderPitch', 'RShoulderRoll', 'RShoulderPitch', 'LHipRoll',
                 'LHipPitch', 'LKneePitch', 'LAnklePitch', 'LAnkleRoll', 'RHipRoll', 'RHipPitch',
                 'RKneePitch', 'RAnklePitch', 'RAnkleRoll']
        angles = [[0.3], [0.4], [-0.5], [1.0], [0.0], [-0.4, -0.2], [0.95, 1.5], [-0.55, -1],
                  [-0.2], [0.0], [-0.4], [0.95], [-0.55], [-0.2]]
        times =  [[0.5], [0.5], [ 0.5], [0.5], [0.5], [ 0.4,  0.8], [ 0.4, 0.8], [ 0.4, 0.8],
                  [ 0.4], [0.5], [ 0.4], [0.4] , [0.4],   [0.4]]

        motion_proxy.angleInterpolation(names, angles, times, True)

        motion_proxy.angleInterpolation(['LShoulderPitch', 'LHipPitch', 'LKneePitch', 'LAnklePitch'],
                                        [1.0, -0.7, 1.05, -0.5],
                                        [[0.1], [0.1], [0.1], [0.1]], True)
        motion_proxy.angleInterpolation(['LHipPitch', 'LKneePitch', 'LAnklePitch'],
                                        [-0.5, 1.1, -0.65],
                                        [[0.25], [0.25], [0.25]], True)
