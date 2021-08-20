/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <frc/Joystick.h>
#include <frc/PWMVictorSPX.h>
#include <frc/TimedRobot.h>
#include <frc/drive/MecanumDrive.h>
#include <studica/TitanQuad.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableEntry.h"
#include "networktables/NetworkTableInstance.h"

/**
 * This is a demo program showing how to use Mecanum control with the
 * MecanumDrive class. If using the VMX-Pi, do not forget to add the vendor libraries
 * http://dev.studica.com/releases/2020/Studica.json
 */
class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override {
    //We need to use Network Tables as Photon cannot build for the Pi.
    /**auto inst = nt::NetworkTableInstance::GetDefault();
    auto table = inst.GetTable("photonvision/usbCam");
    xEntry = table->GetEntry("taregtPixelsX");
    yEntry = table->GetEntry("targetPixelsY");*/
    
    
    // Invert the left side motors. You may need to change or remove this to
    // match your robot.
    m_frontLeft.SetInverted(true);
    m_rearLeft.SetInverted(true);
  }

  void RobotPeriodic(){
    
    

  }

  void TeleopInit() override {
    //This is a wonderful debug tool. Print to the console.
    wpi::outs() << "Entering Teleop...";
    //frc::SmartDashboard::PutNumber("x",xEntry.GetDouble(0.0));
    
  }

  void TeleopPeriodic() override {
    /* Use the joystick X axis for lateral movement, Y axis for forward
     * movement, and Z axis for rotation.
     */

    //Using shared pointers rather than constructing the table explicitly seemed to work here.
    //The Limelight example used the shared pointer in teleop, so I put it here. It is possible that some or all of this could go into robotInit.
    std::shared_ptr<NetworkTable> table = NetworkTable::GetTable("photonvision/usbCam");
    //Create the variable to hold the table using a pointer. The pointer links to a specific method
    targetX = table->GetEntry("targetPixelsX");
    //Next we update the data in the smart dashboard
    frc::SmartDashboard::PutNumber("tx",targetX.GetDouble(0.0));
    //I also wanted to see if we could use the pointer on multiple objects, so I tried grabbing the alliance data. It worked as well.
    std::shared_ptr<NetworkTable> table2 = NetworkTable::GetTable("FMSInfo");
    
    alliance = table2->GetEntry("IsRedAlliance");
    
    frc::SmartDashboard::PutBoolean("Red Alliance?",alliance.GetBoolean(false));

  



    
    m_robotDrive.DriveCartesian(m_stick.GetX(), m_stick.GetY(), m_stick.GetZ());
  }

 private:
  static constexpr int kFrontLeftChannel = 0;
  static constexpr int kRearLeftChannel = 1;
  static constexpr int kFrontRightChannel = 2;
  static constexpr int kRearRightChannel = 3;

  static constexpr int kJoystickChannel = 0;

  frc::PWMVictorSPX m_frontLeft{kFrontLeftChannel};
  frc::PWMVictorSPX m_rearLeft{kRearLeftChannel};
  frc::PWMVictorSPX m_frontRight{kFrontRightChannel};
  frc::PWMVictorSPX m_rearRight{kRearRightChannel};
  frc::MecanumDrive m_robotDrive{m_frontLeft, m_rearLeft, m_frontRight,
                                 m_rearRight};

  frc::Joystick m_stick{kJoystickChannel};
  nt::NetworkTableEntry xEntry;
  nt::NetworkTableEntry yEntry;
  nt::NetworkTableEntry alliance;
  nt::NetworkTableEntry targetX;

};

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
