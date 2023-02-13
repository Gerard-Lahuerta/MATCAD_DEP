import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Scanner;
import java.util.Random;

public class Univers_2 {
    protected int numBodies;
    protected double radius;
    protected Body[] bodies;

    public Univers_2(String fname) {
        try {
            Scanner in = new Scanner(new FileReader(fname));
            numBodies = Integer.parseInt(in.next());
// to set scale for drawing on screen
            radius = Double.parseDouble(in.next());
// read in the n bodies
            bodies = new Body[numBodies];
            for (int i = 0; i < numBodies; i++) {
                double rx = Double.parseDouble(in.next());
                double ry = Double.parseDouble(in.next());
                double vx = Double.parseDouble(in.next());
                double vy = Double.parseDouble(in.next());
                double mass = Double.parseDouble(in.next());
                double[] position = {rx, ry};
                double[] velocity = {vx, vy};
                Vector r = new Vector(position);
                Vector v = new Vector(velocity);
                bodies[i] = new Body(r, v, mass);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    public Univers_2(int numBodies){
        this.numBodies = numBodies;
        bodies = new Body[numBodies];
        radius = 1.15e11;
        double xpos, ypos, xvel, yvel;
        double Minmass, Maxmass, Finalmass;
        for (int i=0; i < numBodies; i++){
            xpos = Math.random()*10e10-Math.random()*10e10;   // El Math.random genera un numero aleatorio entre (0,1)
            ypos = Math.random()*10e10-Math.random()*10e10;  //Para que se ecuentren en un orden cercano de magnitud
            xvel = Math.random()*10e2;
            yvel = Math.random()*10e2;
            Minmass = 5.97e24; //masa Tierra
            Maxmass = 1.99e30; //masa Sol
            Finalmass = Math.random()*(Maxmass-Minmass); //una medio invención
            double[] pos = {xpos, ypos}; //vectores para guardar posicion
            double[] vel = {xvel, yvel}; // y velocidad
            Vector position = new Vector(pos); //TEBGO SUEÑO
            Vector velocity = new Vector(vel);
            bodies[i] = new Body (position, velocity, Finalmass);
        }
    }

    public void Update(double dt){
        // increment time by dt units, assume forces are constant in given interval

        // initialize the forces to zero
        Vector[] f = new Vector[numBodies];
        for (int i = 0; i < numBodies; i++) {
            f[i] = new Vector(new double[2]);
        }

        // compute the forces
        for (int i = 0; i < numBodies; i++) {
            for (int j = 0; j < numBodies; j++) {
                if (i != j) {
                    f[i] = f[i].plus(bodies[i].forceFrom(bodies[j]));
                }
            }
        }

        // move the bodies
        for (int i = 0; i < numBodies; i++) {
            bodies[i].move(f[i], dt);
        }
    }
}
