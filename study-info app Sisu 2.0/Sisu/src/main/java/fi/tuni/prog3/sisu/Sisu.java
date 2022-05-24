package fi.tuni.prog3.sisu;

import java.io.IOException;

import javafx.application.Application;
import javafx.stage.Stage;

/**
 * Main class
 */
public class Sisu extends Application {

    @Override
    public void start(Stage primaryStage) throws IOException {
    	
    	SisuLogic sisu = new SisuLogic();      
    	SisuUI sisuUI = new SisuUI(sisu);
    	sisuUI.show(primaryStage);
        
    }

    /**
     * Main fuction, starts the program
     * @param args Nothing in this case
     */
    public static void main(String[] args) {
        launch();
    }


}