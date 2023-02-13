import java.util.Random;


public class NBodySimulator {
    private double timeStep;
    private int pauseTime;
    private boolean trace; // static
    public int numBodies;
    public Univers_2 univers;

    public NBodySimulator(String fname, double dt, int pauseTime, boolean do_trace){
        univers = new Univers_2(fname);
        this.pauseTime = pauseTime;
        this.trace = do_trace;
        this.timeStep = dt;
    }
    public NBodySimulator(int numBodies){
        univers = new Univers_2(numBodies);
        this.numBodies=numBodies;
    }

    public void drawUniverse() {
        for (int i = 0; i < univers.bodies.length; i++) {
            // if (i == 1 || i == 2) bodies[i].draw(0.05);
            univers.bodies[i].draw();
        }
    }

    public void simulate(int initTime, int endTime, boolean trace){ //por argumento (muchos cuerpos, para que no pete)
        createCanvas();
        while(true){
            double temps1 = System.currentTimeMillis();
            if (trace){
                StdDraw.setPenColor(StdDraw.WHITE);
                drawUniverse();   //genera trayectoria
            }
            else{
                StdDraw.clear();
            }
            univers.Update(initTime);
            StdDraw.setPenColor(StdDraw.BLACK);
            drawUniverse(); //planetas
            StdDraw.show();
            StdDraw.pause(endTime);
            System.out.println(System.currentTimeMillis()-temps1);
        }
    }

    public void simulate() { //por el main (pocos cuerpos)
        createCanvas();
        while(true){
            double temps1 = System.currentTimeMillis();
            if (this.trace){
              StdDraw.setPenColor(StdDraw.WHITE);
              drawUniverse();   //genera trayectoria
            }
            else{
                StdDraw.clear();
            }
            univers.Update(timeStep);
            StdDraw.setPenColor(StdDraw.BLACK);
            drawUniverse(); //planetas
            StdDraw.show();
            StdDraw.pause(pauseTime);
            System.out.println(System.currentTimeMillis()-temps1);


        }
    }

    private void createCanvas(){    //esto ta bien?
    StdDraw.clear();
    StdDraw.enableDoubleBuffering();
    StdDraw.setCanvasSize(500, 500);
    StdDraw.setXscale(-univers.radius, +univers.radius);
    StdDraw.setYscale(-univers.radius, +univers.radius);

    }

    public static void main(String[] args) {
        NBodySimulator nbody;
        int numargs = args.length;
        if ((numargs==3) || (numargs==4)) {
            double dt = Double.parseDouble(args[0]);
            String fname = args[1];
            int pauseTime = Integer.parseInt(args[2]);
            boolean do_trace = false;
            if (args.length == 4) {
                do_trace = args[3].toLowerCase().equals("trace");
            }
            nbody = new NBodySimulator(fname, dt, pauseTime,
                    do_trace);
            nbody.simulate();
        } else if (numargs==1) {
            int increaseTime = 1000;
            int timePause = 10;
            boolean trace = true;
            int numBodies = Integer.parseInt(args[0]);
            nbody = new NBodySimulator(numBodies);
            nbody.simulate(increaseTime,timePause,trace);
        } else {
            assert false : "invalid number of arguments";
        }

    }
}
