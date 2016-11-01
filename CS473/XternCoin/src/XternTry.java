/**
 * Created by shreyaanshbassi on 10/31/16.
 */
public class XternTry {
    public class XternUser {
        String uName;
        int numOfCoins;
        // Arraylist of Booleans for logging attempts
        // Each time an attempt is made, the result is
        // stored in the attempt ArrayList
        ArrayList<Boolean> attempt;

        public XternUser (String name) {
            uName = name;
            numOfCoins = 0;
            attempt = new ArrayList<Boolean>();
        }
    }

    public class XternCoin {

        private static int number;
        private ArrayList<XternUser> userList;
        private static ServerSocket serverSocket;
        private static Socket clientSocket;

        // Checks if a user with username uid already exists, if he
        // doesn't then a new XternUser is created with the username uid
        // and returned.
        private XternUser checkUser (String name) {
            for (XternUser temp : userList) {
                if (temp.uName.equals(name)) {
                    return temp;
                }
            }
            XternUser tUser = new XternUser(name);
            userList.add(tUser);
            return tUser;
        }

        private void setNumber() {
            // Method to select and assign a random number between 1 to 10
            Random rand = new Random();
            number = rand.nextInt(10) + 1;
        }

        public Boolean HandleGuess(int guess, XternUser user) {
            // Method that returns true if a guess is correct and false if
            // the guess is incorrect
            if (guess == number) {
                // Attempt the log and change the number that is
                // to be guessed by the other users
                user.attempt.push(true);
                setNumber();
                return true;
            }
            user.attempt.push(false);
            return false;
        }

        // Method to get the number of coins a user possesses
        public int GetCoins (XternUser user) {
            return user.numOfCoins;
        }

        // Update the number of coins a user has once he has guessed
        // the correct number
        private void UpdateCoin (XternUser user) {
            user.numOfCoins++;
            return;
        }

        // Automated method when called creates or uses an existing user and
        // tries to guess the correct number indefinitely while accumulating
        // XternCoins on each correct guess
        public void StartGuessing (String uid) {

            // Gets existing user if user with username uid exists
            // or creates a new user with the username uid.
            XternUser user = checkUser(uid);

            while (true) {
                for (int i = 1; i <= 10; i++) {
                    if (HandleGuess(i, user)) {
                        UpdateCoin(user);
                    }
                }
            }

        }

        // Constructor for XternCoin
        public XternCoin() {
            userList = new ArrayList<XternUser>();
        }

        // Main method
        public static void main (String[] args) {
            try {
                serverSocket = new ServerSocket(1234);
            } catch (Exception e) {

            }

            // Open a client socket connecting to the serverSocket
            while (true) {
                try {
                    clientSocket = serverSocket.accept();

                    // Multithreaded server creation by having any new client
                    // run a new thread when connected to the server.
                    Thread t = new Thread(new ClientSocketConnect);
                    t.start();
                } catch (Exception e) {
                    System.out.println("Error in connection attempt");
                }
            }
        }
    }
}
