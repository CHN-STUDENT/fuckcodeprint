//Pet.class
package Homework5;

public class Pet {
    private String name="not set";
    private String color="not set";
    private double price;
    Pet() {}
    Pet(String _name,String _color,double _price) {
        this.name=_name;
        this.color=_color;
        this.price=_price;
    }
    public void set(String _name,String _color,double _price) {
        this.name=_name;
        this.color=_color;
        this.price=_price;
    }
    public void show() {
        System.out.println("I am "+name+" , my color is "+color+" , and my pirce is "+price+" .");
    }
}

class Cat extends Pet {
    Cat() {super();}
    Cat(String _name,String _color,double _price) {
        super(_name,_color,_price);
    }
    @Override
    public void set(String _name,String _color,double _price) {
        super.set(_name,_color,_price);
    }

    @Override
    public void show() {
        super.show();
        System.out.println("Also i can sound like Meow-Meow!");
    }
}

class Dog extends Pet {
    Dog() {super();}
    Dog(String _name,String _color,double _price) {
        super(_name,_color,_price);
    }
    @Override
    public void set(String _name,String _color,double _price) {
        super.set(_name,_color,_price);
    }

    @Override
    public void show() {
        super.show();
        System.out.println("Also i can sound like Wang-Wang!");
    }
}



