
![20150111_124621_s](https://github.com/user-attachments/assets/ed18dff3-0a49-417b-a6f1-a21dc4bd162f)

<h2>Button Sorter</h2>

This software supports a specific physical machine, a prototype robot which sorted the blank caps from lapel-pin buttons (like campaign buttons) into top-side up and bottom-side up stacks for sorting as input into other machines.

The machine sorted a column of button caps stacked in a tube, which had been sorted into the stack by another, entirely passive device. A magnetic end effector descended to the column, and upon detecting contact with a button would lift up the top button from the stack (the magnet being just strong enough to lift one article). The end effector carried the cap to a measuring platform, on which an infrared sensor reading would detect if the cap was top- or bottom- up. Once classified, a sweeping armature on the plate would sweep the button left into one chute if it were top-up, and right into another if bottom-up, then repeat the task.

The robot consisted of several parts:

![223a0b12-c117-4ae9-bbf8-e04dce6772bb](https://github.com/user-attachments/assets/6ffc5f51-95b8-4024-8eea-9976e0fa8e26)

- The end effector, implementing a tunable-strength magnet by mounting the magnet on a threaded post, the height of which from the connection point was adjustable, and a pair of spring-loaded contact probes
- The end effector linkage, a slider-crank mechanism which was driven by a positional servo, with end limits set by bumper-contact switches

- A translation axis, constructed from a decomissioned laser printer and driven by a standard H-bridge, with end-point limiters using bumper-contact switches, and non-contact hall effect sensors for the target stack and measurement plate locations

![20150111_124627](https://github.com/user-attachments/assets/70194cb4-abfc-49fc-8c61-a311a3f9e56c)

- The measurement plate, which collected the button caps from the end effector with magnets substantially stronger than that on the end effector, and with a mounted IR sensor below a plexiglas plate for determining the button cap orientation
- The sweep armature, which used a rubber-coated end to push the button laterally off the magnetized measurement plate, driven by a full rotation servo, which could spin full CW or CCW to push left or right freely, and a hall effect sensor to detect the home position




