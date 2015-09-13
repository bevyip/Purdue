import java.util.*;
class CarLot implements Inventory<Car> {
    // your code goes here
    int lotCapacity = 0;
    ArrayList<Car> lotCap = new ArrayList<Car>();
    CarLot(int lotCapacity) {
        this.lotCapacity = lotCapacity;
    }
    public int capacity() {
        return lotCapacity;
    }
    public int numVehicles() {
        return lotCap.size();
    }
    public boolean add(Car vehicle) {
        if (lotCap.size() < lotCapacity) {
            lotCap.add(vehicle);
            return true;
        } else {
            return false;
        }
    }
    public Car get(int location) {
        if (location >= 0) {
            if (location < lotCap.size()) {
                return lotCap.get(location);
            } else {
                return null;
            }
        } else {
            return null;
        }
    }
    public Car remove(int location) {
        if (location >= 0) {
            if (location < lotCap.size()) {
                return lotCap.remove(location);
            } else {
                return null;
            }
        } else {
            return null;
        }
    }
    public boolean[] searchByMake(String make) {
        boolean[] carmake = new boolean[lotCap.size()];
        for (int i = 0; i < lotCap.size(); i++) {
            if (lotCap.get(i).getMake().equals(make)) {
                carmake[i] = true;
            } else {
                carmake[i] = false;
            }
        }
        return carmake;
    }
}