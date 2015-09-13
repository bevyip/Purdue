class Car implements Vehicle {
  // your code goes here
    String carColor = "";
    String carMake = "";
    public Car(String carColor, String carMake) {
        this.carColor = carColor;
        this.carMake = carMake;
    }
    public String getColor() {
        return carColor;
    }
    public String getMake() {
        return carMake;
    }
}