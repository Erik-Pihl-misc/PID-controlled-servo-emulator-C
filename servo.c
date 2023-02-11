/********************************************************************************
* servo.c: Includes drivers for PID controlled servos with TOF (Time Of Flight)
*          sensors to read the relative angle, followed by regulating the
*          servo angle towards the target.
********************************************************************************/
#include "servo.h"

/* Static functions: */
static inline void servo_print_relative_angle(const struct servo* self,
                                              FILE* ostream);

/********************************************************************************
* servo_init: Initiates servo with specified parameters. For instance, a target
*             of 90 degrees is center, a range of 0 - 180 degrees means all the
*             way to the left and to the right (0 = left, 180 = right).
*
*             - self        : Reference to the servo.
*             - target_angle: Target angle for servo.
*             - angle_min   : Minimum servo angle.
*             - angle_max   : Maximum servo angle.
*             - input_min   : Minimum input value for sensors.
*             - input_max   : Maximum input value for sensors.
*             - kp          : Proportional constant for PID controller.
*             - ki          : Integrate constant for PID controller.
*             - kd          : Derivate constant for PID controlle.
********************************************************************************/
void servo_init(struct servo* self,
                const double target_angle,
                const double angle_min,
                const double angle_max,
                const double input_min,
                const double input_max,
                const double kp,
                const double ki,
                const double kd)
{
   pid_init(&self->pid, target_angle, angle_min, angle_max, kp, ki, kd);
   tof_init(&self->tof_left, input_min, input_max);
   tof_init(&self->tof_right, input_min, input_max);
   return;
}

/********************************************************************************
* servo_print: Prints target value, input and output for servo, along with servo
*              angle relative to the target. The output is printed with one
*              decimal per parameter. As default, the output is printed in
*              the terminal.
*
*              - self   : Reference to the servo.
*              - ostream: Reference to output stream used (default = stdout).
********************************************************************************/
void servo_print(const struct servo* self,
                 FILE* ostream)
{
   if (!ostream) ostream = stdout;
   fprintf(ostream, "--------------------------------------------------------------------------------\n");
   fprintf(ostream, "Target servo angle:\t\t%.1f\n", servo_target(self));
   fprintf(ostream, "Mapped input angle:\t\t%.1f\n", servo_input_mapped(self));
   fprintf(ostream, "Current servo angle:\t\t%.1f\n\n", servo_output(self));
   servo_print_relative_angle(self, ostream);
   fprintf(ostream, "--------------------------------------------------------------------------------\n\n");
   return;
}

/********************************************************************************
* servo_print_relative_angle: Prints the servo angle relative to target with
*                             one decimal. As default, the output is printed in
*                             the terminal.
*
*                             - self   : Reference to the servo.
*                             - ostream: Reference to output stream used
*                                        (default = stdout).
********************************************************************************/
static inline void servo_print_relative_angle(const struct servo* self,
                                              FILE* ostream)
{
   if (servo_output(self) < servo_target(self))
   {
      fprintf(ostream, "The servo is angled %.1f degrees to the left of target!\n",
              servo_target(self) - servo_output(self));
   }
   else if (servo_output(self) > servo_target(self))
   {
      fprintf(ostream, "The servo is angled %.1f degrees to the right of target!\n",
              servo_output(self) - servo_target(self));
   }
   else
   {
      fprintf(ostream, "The servo is angled right at target!\n");
   }
   return;
}

/********************************************************************************
* servo_run: Reads input values for left and right TOF sensor, regulates the
*            servo angle according to the input and prints the result in the
*            terminal.
*
*            - self: Reference to the servo.
********************************************************************************/
void servo_run(struct servo* self)
{
   printf("Enter input for left sensor:\n");
   tof_read_from_terminal(&self->tof_left);
   printf("Enter input for right sensor:\n");
   tof_read_from_terminal(&self->tof_right);

   pid_regulate(&self->pid, servo_input_mapped(self));
   servo_print(self, 0);
   return;
}