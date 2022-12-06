## Spring_day02

> * 掌握 IOC/DI 配置管理第三方 bean
> * 掌握 IOC/DI 的注解开发
> * 掌握 IOC/DI 注解管理第三方 bean
> * 完成 Spring 与 Mybatis 及 Junit 的整合开发

## 1、IOC/DI配置管理第三方bean

前面所讲的知识点都是基于我们自己写的类，现在如果有需求让我们去管理第三方 jar 包中的类，该如何管理？

### 1.1 案例：数据源对象管理

在这一节中，我们将通过一个案例来学习下对于第三方 bean 该如何进行配置管理。

以后我们会用到很多第三方的 bean，本次案例将使用咱们前面提到过的数据源`Druid(德鲁伊)`和`C3P0`来配置学习下。

#### 1.1.1 环境准备

学习之前，先来准备下案例环境：

* 创建一个 Maven 项目

  ![1629860338328](assets/1629860338328.png)

* pom.xml 添加依赖

  ```xml
  <dependencies>
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-context</artifactId>
          <version>5.2.10.RELEASE</version>
      </dependency>
  </dependencies>
  ```

* resources 下添加 spring 的配置文件 applicationContext.xml

  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <beans xmlns="http://www.springframework.org/schema/beans"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="
              http://www.springframework.org/schema/beans
              http://www.springframework.org/schema/beans/spring-beans.xsd">
  
  </beans>
  ```
  
* 编写一个运行类 App

  ```java
  public class App {
      public static void main(String[] args) {
          ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
      }
  }
  ```

#### 1.1.2 思路分析

在上述环境下，我们来对数据源进行配置管理，先来分析下思路：

> 需求：使用 Spring 的 IOC 容器来管理 Druid 连接池对象
>
> 1. 使用第三方的技术，需要在 pom.xml 添加依赖
>
> 2. 在配置文件中将【第三方的类】制作成一个 bean，让 IOC 容器进行管理
>
> 3. 数据库连接需要基础的四要素`驱动`、`连接`、`用户名`和`密码`，【如何注入】到对应的 bean 中
>
> 4. 从 IOC 容器中获取对应的 bean 对象，将其打印到控制台查看结果

**思考：**

* 第三方的类指的是什么？
* 如何注入数据库连接四要素？

#### 1.1.3 实现Druid管理

带着这两个问题，把下面的案例实现下：

##### 步骤1：导入`druid`的依赖

pom.xml 中添加依赖

```xml
<dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>druid</artifactId>
    <version>1.1.16</version>
</dependency>
```

##### 步骤2：配置第三方bean

在 applicationContext.xml 配置文件中添加`DruidDataSource`的配置

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="
            http://www.springframework.org/schema/beans
            http://www.springframework.org/schema/beans/spring-beans.xsd">
	<!-- 管理 DruidDataSource 对象-->
    <bean class="com.alibaba.druid.pool.DruidDataSource">
        <property name="driverClassName" value="com.mysql.jdbc.Driver"/>
        <property name="url" value="jdbc:mysql://localhost:3306/spring_db"/>
        <property name="username" value="root"/>
        <property name="password" value="root"/>
    </bean>
</beans>
```

**说明：**

* driverClassName：数据库驱动
* url：数据库连接地址
* username：数据库连接用户名
* password：数据库连接密码
* 数据库连接的四要素要和自己使用的数据库信息一致

##### 步骤3：从IOC容器中获取对应的bean对象

```java
public class App {
    public static void main(String[] args) {
       ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
       DataSource dataSource = (DataSource) ctx.getBean("dataSource");
       System.out.println(dataSource);
    }
}
```

##### 步骤4：运行程序

打印如下结果：说明第三方 bean 对象已经被 spring 的 IOC 容器进行管理

![1629887733081](assets/1629887733081.png)

做完案例后，我们可以将刚才思考的两个问题答案说下：

- 第三方的类指的是什么？

  ```
  DruidDataSource
  ```

- 如何注入数据库连接四要素？

  ```
  setter注入
  ```

#### 1.1.4 实现C3P0管理

完成了 DruidDataSource 的管理，接下来我们再来加深下练习，这次我们来管理`C3P0`数据源，具体的实现步骤是什么呢？

>需求：使用 Spring 的 IOC 容器来管理 C3P0 连接池对象
>
>实现方案和上面基本一致，重点要关注管理的是哪个 bean 对象？

##### 步骤1：导入`C3P0`的依赖

pom.xml 中添加依赖

```xml
<dependency>
    <groupId>c3p0</groupId>
    <artifactId>c3p0</artifactId>
    <version>0.9.1.2</version>
</dependency>
```

对于新的技术，不知道具体的坐标该如何查找？

* 直接百度搜索

* 从 mvn 的仓库`https://mvnrepository.com/`中进行搜索

  ![1629888540286](assets/1629888540286.png)

##### 步骤2：配置第三方bean

在 applicationContext.xml 配置文件中添加配置

```xml
<bean id="dataSource" class="com.mchange.v2.c3p0.ComboPooledDataSource">
    <property name="driverClass" value="com.mysql.jdbc.Driver"/>
    <property name="jdbcUrl" value="jdbc:mysql://localhost:3306/spring_db"/>
    <property name="user" value="root"/>
    <property name="password" value="root"/>
    <property name="maxPoolSize" value="1000"/>
</bean>
```

**==注意：==**

* ComboPooledDataSource 的属性是通过 setter 方式进行注入
* 想注入属性就需要在 ComboPooledDataSource 类或其上层类中有提供属性对应的 setter 方法
* C3P0 的四个属性和 Druid 的四个属性是不一样的

##### 步骤3：运行程序

程序会报错，错误如下

![1629889170229](assets/1629889170229.png)

报的错为==ClassNotFoundException==，翻译出来是`类没有发现的异常`，具体的类为`com.mysql.jdbc.Driver`。错误的原因是缺少 mysql 的驱动包。

分析出错误的原因，具体的解决方案就比较简单，只需要在 pom.xml 把驱动包引入即可。

```xml
<dependency>
    <groupId>mysql</groupId>
    <artifactId>mysql-connector-java</artifactId>
    <version>5.1.47</version>
</dependency>
```

添加完 mysql 的驱动包以后，再次运行 App，就可以打印出结果：

![1629903845404](assets/1629903845404.png)

**注意：**

* 数据连接池在配置属性的时候，除了可以注入数据库连接四要素外还可以配置很多其他的属性，具体都有哪些属性用到的时候再去查，一般配置基础的四个，其他都有自己的默认值
* Druid 和 C3P0 在没有导入 mysql 驱动包的前提下，一个没报错一个报错，说明 Druid 在初始化的时候没有去加载驱动，而 C3P0 刚好相反
* Druid 程序运行虽然没有报错，但是当调用 DruidDataSource 的 getConnection() 方法获取连接的时候，也会报找不到驱动类的错误

### 1.2 加载properties文件

上节中我们已经完成两个数据源`druid`和`C3P0`的配置，但是其中包含了一些问题，我们来分析下：

* 这两个数据源中都使用到了一些固定的常量如数据库连接四要素，把这些值写在 Spring 的配置文件中不利于后期维护
* 需要将这些值提取到一个外部的 properties 配置文件中
* Spring 框架如何从配置文件中读取属性值来配置就是接下来要解决的问题

问题提出来后，具体该如何实现？

#### 1.2.1 第三方bean属性优化

##### 1.2.1.1 实现思路

> 需求：将数据库连接四要素提取到 properties 配置文件，spring 来加载配置信息并使用这些信息来完成属性注入。
>
> 1. 在 resources 下创建一个 jdbc.properties（文件的名称可以任意）
>
> 2. 将数据库连接四要素配置到配置文件中
>
> 3. 在 Spring 的配置文件中加载 properties 文件
>
> 4. 使用加载到的值实现属性注入
>
> 其中第3，4步骤是需要大家重点关注，具体是如何实现。

##### 1.2.1.2 实现步骤

###### 步骤1：准备properties配置文件

resources 下创建一个 `jdbc.properties` 文件，并添加对应的属性键值对

```properties
jdbc.driver=com.mysql.jdbc.Driver
jdbc.url=jdbc:mysql://127.0.0.1:3306/spring_db
jdbc.username=root
jdbc.password=root
```

###### 步骤2：开启`context`命名空间

在 applicationContext.xml 中开`context`命名空间

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xsi:schemaLocation="
            http://www.springframework.org/schema/beans
            http://www.springframework.org/schema/beans/spring-beans.xsd
            http://www.springframework.org/schema/context
            http://www.springframework.org/schema/context/spring-context.xsd">
</beans>
```

###### 步骤3：加载properties配置文件

在配置文件中使用`context`命名空间下的标签来加载 properties 配置文件

```xml
<context:property-placeholder location="jdbc.properties"/>
```

###### 步骤4：完成属性注入

使用`${key}`来读取 properties 配置文件中的内容并完成属性注入

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xsi:schemaLocation="
            http://www.springframework.org/schema/beans
            http://www.springframework.org/schema/beans/spring-beans.xsd
            http://www.springframework.org/schema/context
            http://www.springframework.org/schema/context/spring-context.xsd">
    
    <context:property-placeholder location="jdbc.properties"/>
    <bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource">
        <property name="driverClassName" value="${jdbc.driver}"/>
        <property name="url" value="${jdbc.url}"/>
        <property name="username" value="${jdbc.username}"/>
        <property name="password" value="${jdbc.password}"/>
    </bean>
</beans>
```

至此，读取外部 properties 配置文件中的内容就已经完成。

#### 1.2.2 读取单个属性

##### 1.2.2.1 实现思路

对于上面的案例，效果不是很明显，我们可以换个案例来演示下：

> 需求：从 properties 配置文件中读取 key 为 name 的值，并将其注入到 BookDao 中并在 save 方法中进行打印。
>
> 1. 在项目中添加 BookDao 和 BookDaoImpl 类
>
> 2. 为 BookDaoImpl 添加一个 name 属性并提供 setter 方法
>
> 3. 在 jdbc.properties 中添加数据注入到 bookDao 中打印方便查询结果
>
> 4. 在 applicationContext.xml 添加配置完成配置文件加载、属性注入（${key}）

##### 1.2.2.2 实现步骤

###### 步骤1：在项目中添对应的类

BookDao 和 BookDaoImpl 类，并在 BookDaoImpl 类中添加`name`属性与 setter 方法

```java
public interface BookDao {
    public void save();
}

public class BookDaoImpl implements BookDao {
    private String name;

    public void setName(String name) {
        this.name = name;
    }

    public void save() {
        System.out.println("book dao save ..." + name);
    }
}
```

###### 步骤2：完成配置文件的读取与注入

在 applicationContext.xml 添加配置，`bean的配置管理`、`读取外部properties`、`依赖注入`：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xsi:schemaLocation="
            http://www.springframework.org/schema/beans
            http://www.springframework.org/schema/beans/spring-beans.xsd
            http://www.springframework.org/schema/context
            http://www.springframework.org/schema/context/spring-context.xsd">
    
    <context:property-placeholder location="jdbc.properties"/>
    
    <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl">
        <property name="name" value="${jdbc.driver}"/>
    </bean>
</beans>
```

###### 步骤3：运行程序

在 App 类中，从 IOC 容器中获取 bookDao 对象，调用方法，查看值是否已经被获取到并打印控制台

```java
public class App {
    public static void main(String[] args) throws Exception{
        ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
        BookDao bookDao = (BookDao) ctx.getBean("bookDao");
        bookDao.save();
    }
}
```

![1629975492444](assets/1629975492444.png)

##### 1.2.2.3 注意事项

至此，读取 properties 配置文件中的内容就已经完成，但是在使用的时候，有些注意事项：

* 问题一：键值对的 key 为`username`引发的问题

  1. 在 properties 中配置键值对的时候，如果 key 设置为`username`

  ```
  username=root666
  ```

  2. 在 applicationContext.xml 注入该属性

  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <beans xmlns="http://www.springframework.org/schema/beans"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xmlns:context="http://www.springframework.org/schema/context"
         xsi:schemaLocation="
              http://www.springframework.org/schema/beans
              http://www.springframework.org/schema/beans/spring-beans.xsd
              http://www.springframework.org/schema/context
              http://www.springframework.org/schema/context/spring-context.xsd">
      
      <context:property-placeholder location="jdbc.properties"/>
      
      <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl">
          <property name="name" value="${username}"/>
      </bean>
  </beans>
  ```

  3. 运行后，在控制台打印的却不是`root666`，而是自己电脑的用户名

  ![1629975934694](assets/1629975934694.png)

  4. 出现问题的原因是`<context:property-placeholder/>`标签会加载系统的环境变量，而且环境变量的值会被优先加载，如何查看系统的环境变量？

  ```java
  public static void main(String[] args) throws Exception{
      Map<String, String> env = System.getenv();
      System.out.println(env);
  }
  ```

  大家可以自行运行，在打印出来的结果中会有一个 `USERNAME=XXX[自己电脑的用户名称]`

  5. 解决方案

  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <beans xmlns="http://www.springframework.org/schema/beans"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xmlns:context="http://www.springframework.org/schema/context"
         xsi:schemaLocation="
              http://www.springframework.org/schema/beans
              http://www.springframework.org/schema/beans/spring-beans.xsd
              http://www.springframework.org/schema/context
              http://www.springframework.org/schema/context/spring-context.xsd">
      
      <context:property-placeholder location="jdbc.properties" system-properties-mode="NEVER"/>
  </beans>
  ```

  system-properties-mode：设置为 NEVER，表示不加载系统属性，就可以解决上述问题。

  当然还有一个解决方案就是避免使用`username`作为属性的`key`。

* 问题二：当有多个 properties 配置文件需要被加载，该如何配置？

  1. 调整下配置文件的内容，在 resources 下添加`jdbc.properties`,`jdbc2.properties`，内容如下：

  jdbc.properties

  ```properties
  jdbc.driver=com.mysql.jdbc.Driver
  jdbc.url=jdbc:mysql://127.0.0.1:3306/spring_db
  jdbc.username=root
  jdbc.password=root
  ```

  jdbc2.properties

  ```properties
  username=root666
  ```

  2. 修改 applicationContext.xml

  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <beans xmlns="http://www.springframework.org/schema/beans"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xmlns:context="http://www.springframework.org/schema/context"
         xsi:schemaLocation="
              http://www.springframework.org/schema/beans
              http://www.springframework.org/schema/beans/spring-beans.xsd
              http://www.springframework.org/schema/context
              http://www.springframework.org/schema/context/spring-context.xsd">
      <!--方式一 -->
      <context:property-placeholder location="jdbc.properties, jdbc2.properties" system-properties-mode="NEVER"/>
      <!--方式二-->
      <context:property-placeholder location="*.properties" system-properties-mode="NEVER"/>
      <!--方式三 -->
      <context:property-placeholder location="classpath:*.properties" system-properties-mode="NEVER"/>
      <!--方式四-->
      <context:property-placeholder location="classpath*:*.properties" system-properties-mode="NEVER"/>
  </beans>	
  ```

  **说明：**

  * 方式一：可以实现，如果配置文件多的话，每个都需要配置
  * 方式二：`*.properties`代表所有以 properties 结尾的文件都会被加载，可以解决方式一的问题，但是不标准
  * 方式三：标准的写法，`classpath:`代表的是从根路径下开始查找，但是只能查询当前项目的根路径
  * 方式四：不仅可以加载当前项目还可以加载当前项目所依赖的所有项目的根路径下的 properties 配置文件

#### 1.2.3 加载properties文件小结

  本节主要讲解的是 properties 配置文件的加载，需要掌握的内容有：

  * 如何开启`context`命名空间

    ![1629980280952](assets/1629980280952.png)

  * 如何加载 properties 配置文件

    ```xml
    <context:property-placeholder location="" system-properties-mode="NEVER"/>
    ```

  * 如何在 applicationContext.xml 引入 properties 配置文件中的值

    ```
    ${key}
    ```

## 2、核心容器

前面已经完成 bean 与依赖注入的相关知识学习，接下来我们主要学习的是 IOC 容器中的==核心容器==。

这里所说的核心容器，大家可以把它简单的理解为`ApplicationContext`，前面虽然已经用到过（IOC 容器），但是并没有系统的学习，接下来咱们从以下几个问题入手来学习下容器的相关知识：

* 如何创建容器？
* 创建好容器后，如何从容器中获取 bean 对象？
* 容器类的层次结构是什么？
* BeanFactory 是什么？

### 2.1 环境准备

在学习和解决上述问题之前，先来准备下案例环境：

* 创建一个 Maven 项目

* pom.xml 添加 Spring 的依赖

  ```xml
  <dependencies>
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-context</artifactId>
          <version>5.2.10.RELEASE</version>
      </dependency>
  </dependencies>
  ```

* resources 下添加 applicationContext.xml

  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <beans xmlns="http://www.springframework.org/schema/beans"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="
              http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
      <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"/>
  </beans>
  ```

* 添加 BookDao 和 BookDaoImpl 类

  ```java
  public interface BookDao {
      public void save();
  }
  public class BookDaoImpl implements BookDao {
      public void save() {
          System.out.println("book dao save ..." );
      }
  }
  ```

* 创建运行类 App

  ```java
  public class App {
      public static void main(String[] args) {
          ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
          BookDao bookDao = (BookDao) ctx.getBean("bookDao");
          bookDao.save();
      }
  }
  ```

最终创建好的项目结构如下：

![1629982672522](assets/1629982672522.png)

### 2.2 容器

#### 2.2.1 容器的创建方式

案例中创建`ApplicationContext`的方式为：

```java
ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
```

这种方式翻译为：==类路径下的XML配置文件==（推荐）

除了上面这种方式，Spring 还提供了另外一种创建方式为：

```java
ApplicationContext ctx = new FileSystemXmlApplicationContext("applicationContext.xml");
```

这种方式翻译为：==文件系统下的XML配置文件==（不推荐）

使用这种方式，运行，会出现如下错误：

![1629983245121](assets/1629983245121.png)

从错误信息中能发现，这种方式是从项目路径下开始查找`applicationContext.xml`配置文件的，所以需要将其修改为：

```java
ApplicationContext ctx = new FileSystemXmlApplicationContext("D:\\workspace\\spring\\spring_10_container\\src\\main\\resources\\applicationContext.xml"); 
```

**说明：**大家练习的时候，写自己的具体路径。

这种方式虽能实现，但是当项目的位置发生变化后，代码也需要跟着改，耦合度较高，不推荐使用。

#### 2.2.2 Bean的三种获取方式

方式一，就是目前案例中获取的方式：

```java
BookDao bookDao = (BookDao) ctx.getBean("bookDao");
```

这种方式存在的问题是每次获取的时候都需要进行类型转换，有没有更简单的方式呢？

方式二：

```java
BookDao bookDao = ctx.getBean("bookDao"，BookDao.class);
```

这种方式可以解决类型强转问题，但是参数又多加了一个，相对来说没有简化多少。

方式三：

```java
BookDao bookDao = ctx.getBean(BookDao.class);
```

这种方式就类似我们之前所学习依赖注入中的按类型注入。必须要确保 IOC 容器中该类型对应的 bean 对象只能有一个。

#### 2.2.3 容器类层次结构

(1) 在 IDEA 中双击`shift`，输入 BeanFactory

![1629985148294](assets/1629985148294.png)

(2) 点击进入 BeanFactory 类，ctrl+h，就能查看到如下结构的层次关系

![1629984980781](assets/1629984980781.png)

从图中可以看出，容器类也是从无到有根据需要一层层叠加上来的，大家重点理解下这种设计思想。

#### 2.2.4 BeanFactory的使用

使用 BeanFactory 来创建 IOC 容器的具体实现方式为：

```java
public class AppForBeanFactory {
    public static void main(String[] args) {
        Resource resources = new ClassPathResource("applicationContext.xml");
        BeanFactory bf = new XmlBeanFactory(resources);
        BookDao bookDao = bf.getBean(BookDao.class);
        bookDao.save();
    }
}
```

为了更好的看出`BeanFactory`和`ApplicationContext`之间的区别，在 BookDaoImpl 添加如下构造函数：

```java
public class BookDaoImpl implements BookDao {
    public BookDaoImpl() {
        System.out.println("constructor");
    }
    public void save() {
        System.out.println("book dao save ..." );
    }
}
```

如果不去获取 bean 对象，打印会发现：

* BeanFactory 是延迟加载，只有在获取 bean 对象的时候才会去创建

* ApplicationContext 是立即加载，容器加载的时候就会创建 bean 对象

* ApplicationContext 要想成为延迟加载，只需要按照如下方式进行配置

  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <beans xmlns="http://www.springframework.org/schema/beans"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="
              http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
      <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"  lazy-init="true"/>
  </beans>
  ```

**小结**

这一节中所讲的知识点包括：

* 容器创建的两种方式

  * ClassPathXmlApplicationContext[掌握]
  * FileSystemXmlApplicationContext[知道即可]

* 获取 Bean 的三种方式

  * getBean("名称"):需要类型转换
  * getBean("名称",类型.class):多了一个参数
  * getBean(类型.class):容器中不能有多个该类的bean对象

  上述三种方式，各有各的优缺点，用哪个都可以。

* 容器类层次结构

  * 只需要知晓容器的最上级的父接口为 BeanFactory 即可

* BeanFactory

  * 使用 BeanFactory 创建的容器是延迟加载
  * 使用 ApplicationContext 创建的容器是立即加载
  * 具体 BeanFactory 如何创建只需要了解即可

### 2.2 核心容器总结

这节中没有新的知识点，只是对前面知识的一个大总结，共包含如下内容：

#### 2.2.1 容器相关

- BeanFactory 是 IoC 容器的顶层接口，初始化 BeanFactory 对象时，加载的 bean 延迟加载
- ApplicationContext 接口是 Spring 容器的核心接口，初始化时 bean 立即加载
- ApplicationContext 接口提供基础的 bean 操作相关方法，通过其他接口扩展其功能
- ApplicationContext 接口常用初始化类
  - **==ClassPathXmlApplicationContext（常用）==**
  - FileSystemXmlApplicationContext

#### 2.2.2 bean相关

![1629986510487](assets/1629986510487.png)

其实整个配置中最常用的就两个属性==id==和==class==。

把 scope、init-method、destroy-method 框起来的原因是，后面注解在讲解的时候还会用到，所以大家对这三个属性关注下。

#### 2.2.3 依赖注入相关

![1629986848563](assets/1629986848563.png)

## 3、IOC/DI注解开发

Spring 的 IOC/DI 对应的配置开发就已经讲解完成，但是使用起来相对来说还是比较复杂的，复杂的地方在==配置文件==。

前面咱们聊 Spring 的时候说过，Spring 可以简化代码的开发，到现在并没有体会到。

所以 Spring 到底是如何简化代码开发的呢？

要想真正简化开发，就需要用到 Spring 的注解开发，Spring 对注解支持的版本历程：

* 2.0 版开始支持注解
* 2.5 版注解功能趋于完善
* 3.0 版支持纯注解开发

关于注解开发，我们会讲解两块内容`注解开发定义bean`和`纯注解开发`。

注解开发定义 bean 用的是 2.5 版提供的注解，纯注解开发用的是 3.0 版提供的注解。

### 3.1 环境准备

在学习注解开发之前，先来准备下案例环境：

- 创建一个 Maven 项目

- pom.xml 添加 Spring 的依赖

  ```xml
  <dependencies>
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-context</artifactId>
          <version>5.2.10.RELEASE</version>
      </dependency>
  </dependencies>
  ```

- resources 下添加 applicationContext.xml

  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <beans xmlns="http://www.springframework.org/schema/beans"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="
              http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
      <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"/>
  </beans>
  ```

- 添加 BookDao、BookDaoImpl、BookService、BookServiceImpl 类

  ```java
  public interface BookDao {
      public void save();
  }
  public class BookDaoImpl implements BookDao {
      public void save() {
          System.out.println("book dao save ..." );
      }
  }
  public interface BookService {
      public void save();
  }
  
  public class BookServiceImpl implements BookService {
      public void save() {
          System.out.println("book service save ...");
      }
  }
  ```
  
- 创建运行类 App

  ```java
  public class App {
      public static void main(String[] args) {
          ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
          BookDao bookDao = (BookDao) ctx.getBean("bookDao");
          bookDao.save();
      }
  }
  ```

最终创建好的项目结构如下：

![1629989221808](assets/1629989221808.png)

### 3.2 注解开发定义bean

在上述环境的基础上，我们来学一学 Spring 是如何通过注解实现 bean 的定义开发？

#### 步骤1：删除原XML配置

将配置文件中的`<bean>`标签删除掉

```xml
<bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"/>
```

#### 步骤2：Dao上添加注解

在 BookDaoImpl 类上添加`@Component`注解

```java
@Component("bookDao")
public class BookDaoImpl implements BookDao {
    public void save() {
        System.out.println("book dao save ..." );
    }
}
```

==注意：@Component 注解不可以添加在接口上，因为接口是无法创建对象的。==

XML 与注解配置的对应关系：

![1629990315619](assets/1629990315619.png)

#### 步骤3：配置Spring的注解包扫描

为了让 Spring 框架能够扫描到写在类上的注解，需要在配置文件上进行包扫描

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="
            http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
    <context:component-scan base-package="com.itheima"/>
</beans>
```

**说明:**

component-scan

* component：组件，Spring 将管理的 bean 视作自己的一个组件
* scan：扫描

base-package 指定 Spring 框架扫描的包路径，它会扫描指定包及其子包中的所有类上的注解。

* 包路径越多[如：com.itheima.dao.impl]，扫描的范围越小速度越快
* 包路径越少[如：com.itheima]，扫描的范围越大速度越慢
* 一般扫描到项目的组织名称即 Maven 的 groupId 下[如：com.itheima]即可

#### 步骤4：运行程序

运行`App`类查看打印结果

![1630027590558](assets/1630027590558.png)

#### 步骤5：Service上添加注解

在 BookServiceImpl 类上也添加`@Component`交给 Spring 框架管理

```java
@Component
public class BookServiceImpl implements BookService {

    public void save() {
        System.out.println("book service save ...");
    }
}
```

#### 步骤6：运行程序

在 App 类中，从 IOC 容器中获取 BookServiceImpl 对应的 bean 对象，打印

```java
public class App {
    public static void main(String[] args) {
        ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
        BookDao bookDao = (BookDao) ctx.getBean("bookDao");
        System.out.println(bookDao);
        // 按类型获取bean
        BookService bookService = ctx.getBean(BookService.class);
        System.out.println(bookService);
    }
}
```

打印观察结果，两个 bean 对象都已经打印到控制台

![1630027743910](assets/1630027743910.png)

**说明：**

* BookServiceImpl 类没有起名称，所以在 App 中是按照类型来获取 bean 对象

* @Component 注解如果不起名称，会有一个默认值就是`当前类名首字母小写`，所以也可以按照名称获取，如：

  ```java
  BookService bookService = (BookService)ctx.getBean("bookServiceImpl");
  System.out.println(bookService);
  ```

对于 @Component 注解，还衍生出了其他三个注解`@Controller`、`@Service`、`@Repository`

通过查看源码会发现：

![1630028345074](assets/1630028345074.png)

这三个注解和 @Component 注解的作用是一样的，为什么要衍生出这三个呢？

方便我们后期在编写类的时候能很好的区分出这个类是属于`表现层`、`业务层`还是`数据层`的类。

#### 知识点1：@Component等

| 名称 | @Component/@Controller/@Service/@Repository |
| ---- | ------------------------------------------- |
| 类型 | 类注解                                      |
| 位置 | 类定义上方                                  |
| 作用 | 设置该类为 spring 管理的 bean               |
| 属性 | value（默认）：定义 bean 的 id              |

### 3.2 纯注解开发模式

上面已经可以使用注解来配置 bean，但是依然有用到配置文件，在配置文件中对包进行了扫描，Spring 在 3.0 版已经支持纯注解开发

* Spring3.0 开启了纯注解开发模式，使用 Java 类替代配置文件，开启了 Spring 快速开发赛道

具体如何实现？

#### 3.2.1 思路分析

实现思路为：

* 将配置文件 applicationContext.xml 删除掉，使用类来替换。

#### 3.2.2 实现步骤

##### 步骤1：创建配置类

创建一个配置类`SpringConfig`

```java
public class SpringConfig {
}
```

##### 步骤2：标识该类为配置类

在配置类上添加`@Configuration`注解，将其标识为一个配置类，替换`applicationContext.xml`

```java
@Configuration
public class SpringConfig {
}
```

##### 步骤3：用注解替换包扫描配置

在配置类上添加包扫描注解`@ComponentScan`替换`<context:component-scan base-package=""/>`

```java
@Configuration
@ComponentScan("com.itheima")
public class SpringConfig {
}
```

##### 步骤4：创建运行类并执行

创建一个新的运行类`AppForAnnotation`

```java
public class AppForAnnotation {

    public static void main(String[] args) {
        // 加载配置类初始化容器
        ApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
        BookDao bookDao = (BookDao) ctx.getBean("bookDao");
        System.out.println(bookDao);
        BookService bookService = ctx.getBean(BookService.class);
        System.out.println(bookService);
    }
}
```

运行 AppForAnnotation，可以看到两个对象依然被获取成功

![1630029110506](assets/1630029110506.png)

至此，纯注解开发的方式就已经完成了，主要内容包括：

* Java 类替换 Spring 核心配置文件

  ![1630029254372](assets/1630029254372.png)

* @Configuration 注解用于设定当前类为配置类

* @ComponentScan 注解用于设定扫描路径，此注解只能添加一次，多个数据请用数组格式

  ```java
  @ComponentScan({com.itheima.service", "com.itheima.dao"})
  ```

* 读取 Spring 核心配置文件初始化容器对象切换为读取 Java 配置类初始化容器对象

  ```java
  // 加载配置文件初始化容器
  ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
  // 加载配置类初始化容器
  ApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
  ```

#### 知识点1：@Configuration

| 名称 | @Configuration                 |
| ---- | ------------------------------ |
| 类型 | 类注解                         |
| 位置 | 类定义上方                     |
| 作用 | 设置该类为 spring 配置类       |
| 属性 | value（默认）：定义 bean 的 id |

#### 知识点2：@ComponentScan

| 名称 | @ComponentScan                                              |
| ---- | ----------------------------------------------------------- |
| 类型 | 类注解                                                      |
| 位置 | 类定义上方                                                  |
| 作用 | 设置 spring 配置类扫描路径，用于加载使用注解格式定义的 bean |
| 属性 | value（默认）：扫描路径，此路径可以逐层向下扫描             |

**小结：**

这一节重点掌握的是使用注解完成 Spring 的 bean 管理，需要掌握的内容为：

* 记住 @Component、@Controller、@Service、@Repository 这四个注解
* applicationContext.xml 中`<context:component-san/>`的作用是指定扫描包路径，注解为 @ComponentScan
* @Configuration 标识该类为配置类，使用类替换 applicationContext.xml 文件
* ClassPathXmlApplicationContext 是加载 XML 配置文件
* AnnotationConfigApplicationContext 是加载配置类

### 3.3 注解开发bean作用范围与生命周期管理

使用注解已经完成了 bean 的管理，接下来按照前面所学习的内容，将通过配置实现的内容都换成对应的注解实现，包含两部分内容：`bean作用范围`和`bean生命周期`。

#### 3.3.1 环境准备

老规矩，学习之前先来准备环境：

- 创建一个 Maven 项目

- pom.xml 添加 Spring 的依赖

  ```xml
  <dependencies>
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-context</artifactId>
          <version>5.2.10.RELEASE</version>
      </dependency>
  </dependencies>
  ```

- 添加一个配置类`SpringConfig`

  ```java
  @Configuration
  @ComponentScan("com.itheima")
  public class SpringConfig {
  }
  ```

- 添加 BookDao、BookDaoImpl 类

  ```java
  public interface BookDao {
      public void save();
  }
  
  @Repository
  public class BookDaoImpl implements BookDao {
      public void save() {
          System.out.println("book dao save ..." );
      }
  }
  ```

- 创建运行类 App

  ```java
  public class App {
      public static void main(String[] args) {
          AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
          BookDao bookDao1 = ctx.getBean(BookDao.class);
          BookDao bookDao2 = ctx.getBean(BookDao.class);
          System.out.println(bookDao1);
          System.out.println(bookDao2);
      }
  }
  ```

最终创建好的项目结构如下：

![1630031112993](assets/1630031112993.png)

#### 3.3.2 Bean的作用范围

(1) 先运行 App 类，在控制台打印两个一摸一样的地址，说明默认情况下 bean 是单例

![1630031192753](assets/1630031192753.png)

(2) 要想将 BookDaoImpl 变成非单例，只需要在其类上添加`@scope`注解

```java
@Repository
// @Scope设置 bean 的作用范围
@Scope("prototype")
public class BookDaoImpl implements BookDao {

    public void save() {
        System.out.println("book dao save ...");
    }
}
```

再次执行 App 类，打印结果：

![1630031808947](assets/1630031808947.png)

##### 知识点1：@Scope

| 名称 | @Scope                                                       |
| ---- | ------------------------------------------------------------ |
| 类型 | 类注解                                                       |
| 位置 | 类定义上方                                                   |
| 作用 | 设置该类创建对象的作用范围<br/>可用于设置创建出的 bean 是否为单例对象 |
| 属性 | value（默认）：定义 bean 作用范围，<br/>==默认值 singleton（单例），可选值 prototype（非单例）== |

#### 3.3.3 Bean的生命周期

(1) 在 BookDaoImpl 中添加两个方法，`init`和`destroy`，方法名可以任意

```java
@Repository
public class BookDaoImpl implements BookDao {
    public void save() {
        System.out.println("book dao save ...");
    }
    public void init() {
        System.out.println("init ...");
    }
    public void destroy() {
        System.out.println("destroy ...");
    }
}
```

(2) 如何对方法进行标识，哪个是初始化方法，哪个是销毁方法？

只需要在对应的方法上添加`@PostConstruct`和`@PreDestroy`注解即可。

```java
@Repository
public class BookDaoImpl implements BookDao {
    public void save() {
        System.out.println("book dao save ...");
    }
    @PostConstruct // 在构造方法之后执行，替换 init-method
    public void init() {
        System.out.println("init ...");
    }
    @PreDestroy // 在销毁方法之前执行,替换 destroy-method
    public void destroy() {
        System.out.println("destroy ...");
    }
}
```

(3) 要想看到两个方法执行，需要注意的是`destroy`只有在容器关闭的时候，才会执行，所以需要修改 App 的类

```java
public class App {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
        BookDao bookDao1 = ctx.getBean(BookDao.class);
        BookDao bookDao2 = ctx.getBean(BookDao.class);
        System.out.println(bookDao1);
        System.out.println(bookDao2);
        ctx.close(); // 关闭容器
    }
}
```

(4) 运行 App，类查看打印结果，证明 init 和 destroy 方法都被执行了。

![1630032385498](assets/1630032385498.png)

==**注意：**@PostConstruct 和 @PreDestroy 注解如果找不到，需要导入下面的 jar 包==

```java
<dependency>
  <groupId>javax.annotation</groupId>
  <artifactId>javax.annotation-api</artifactId>
  <version>1.3.2</version>
</dependency>
```

找不到的原因是，从 JDK9 以后 jdk 中的 javax.annotation 包被移除了，这两个注解刚好就在这个包中。

##### 知识点1：@PostConstruct

| 名称 | @PostConstruct         |
| ---- | ---------------------- |
| 类型 | 方法注解               |
| 位置 | 方法上                 |
| 作用 | 设置该方法为初始化方法 |
| 属性 | 无                     |

##### 知识点2：@PreDestroy

| 名称 | @PreDestroy          |
| ---- | -------------------- |
| 类型 | 方法注解             |
| 位置 | 方法上               |
| 作用 | 设置该方法为销毁方法 |
| 属性 | 无                   |

**小结**

![1630033039358](assets/1630033039358.png)

### 3.4 注解开发依赖注入

Spring 为了使用注解简化开发，并没有提供`构造函数注入`、`setter注入`对应的注解，只提供了自动装配的注解实现。

#### 3.4.1 环境准备

在学习之前，把案例环境介绍下：

- 创建一个 Maven 项目

- pom.xml 添加 Spring 的依赖

  ```xml
  <dependencies>
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-context</artifactId>
          <version>5.2.10.RELEASE</version>
      </dependency>
  </dependencies>
  ```

- 添加一个配置类`SpringConfig`

  ```java
  @Configuration
  @ComponentScan("com.itheima")
  public class SpringConfig {
  }
  ```

- 添加 BookDao、BookDaoImpl、BookService、BookServiceImpl 类

  ```java
  public interface BookDao {
      public void save();
  }
  @Repository
  public class BookDaoImpl implements BookDao {
      public void save() {
          System.out.println("book dao save ..." );
      }
  }
  public interface BookService {
      public void save();
  }
  @Service
  public class BookServiceImpl implements BookService {
      private BookDao bookDao;
      
  	public void setBookDao(BookDao bookDao) {
          this.bookDao = bookDao;
      }
      
      public void save() {
          System.out.println("book service save ...");
          bookDao.save();
      }
  }
  ```

- 创建运行类 App

  ```java
  public class App {
      public static void main(String[] args) {
          AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
          BookService bookService = ctx.getBean(BookService.class);
          bookService.save();
      }
  }
  ```

最终创建好的项目结构如下：

![1630033604129](assets/1630033604129.png)

环境准备好后，运行后会发现有问题

![1630033710052](assets/1630033710052.png)

出现问题的原因是，在 BookServiceImpl 类中添加了 BookDao 的属性，并提供了 setter 方法，但是目前是没有提供配置注入BookDao 的，所以 bookDao 对象为 Null，调用其 save 方法就会报`控指针异常`。

#### 3.4.2 注解实现按照类型注入

对于这个问题使用注解该如何解决？

(1) 在 BookServiceImpl 类的 bookDao 属性上添加`@Autowired`注解

```java
@Service
public class BookServiceImpl implements BookService {
    @Autowired
    private BookDao bookDao;
    
//	  public void setBookDao(BookDao bookDao) {
//        this.bookDao = bookDao;
//    }
    
    public void save() {
        System.out.println("book service save ...");
        bookDao.save();
    }
}
```

**注意：**

* @Autowired 可以写在属性上，也可也写在 setter 方法上，最简单的处理方式是**写在属性上并将 setter 方法删除掉**
* 为什么 setter 方法可以删除呢？
  * 自动装配基于反射设计创建对象并通过暴力反射为私有属性进行设值
  * 普通反射只能获取 public 修饰的内容
  * 暴力反射除了获取 public 修饰的内容还可以获取 private 修改的内容
  * 所以此处无需提供 setter 方法

(2) @Autowired 是按照类型注入，那么对应 BookDao 接口如果有多个实现类，比如添加 BookDaoImpl2

```java
@Repository
public class BookDaoImpl2 implements BookDao {
    public void save() {
        System.out.println("book dao save ...2");
    }
}
```

这个时候再次运行 App，就会报错

![1630034272959](assets/1630034272959.png)

此时，按照类型注入就无法区分到底注入哪个对象，解决方案：**按照名称注入**

* 先给两个 Dao 类分别起个名称

  ```java
  @Repository("bookDao")
  public class BookDaoImpl implements BookDao {
      public void save() {
          System.out.println("book dao save ..." );
      }
  }
  @Repository("bookDao2")
  public class BookDaoImpl2 implements BookDao {
      public void save() {
          System.out.println("book dao save ...2" );
      }
  }
  ```

  此时就可以注入成功，但是得思考个问题：

  * @Autowired 是按照类型注入的，给 BookDao 的两个实现起了名称，它还是有两个 bean 对象，为什么不报错？

  * @Autowired 默认按照类型自动装配，如果 IOC 容器中同类的 Bean 找到多个，就按照变量名和 Bean 的名称匹配。因为变量名叫`bookDao`而容器中也有一个`booDao`，所以可以成功注入。

  * 分析下面这种情况是否能完成注入呢？

    ![1630036236150](assets/1630036236150.png)

  * 不行，因为按照类型会找到多个 bean 对象，此时会按照`bookDao`名称去找，因为 IOC 容器只有名称叫`bookDao1`和`bookDao2`，所以找不到，会报`NoUniqueBeanDefinitionException`

#### 3.4.3 注解实现按照名称注入

当根据类型在容器中找到多个 bean，注入参数的属性名又和容器中 bean 的名称不一致，这个时候该如何解决，就需要使用到`@Qualifier`来指定注入哪个名称的 bean 对象。

```java
@Service
public class BookServiceImpl implements BookService {
    @Autowired
    @Qualifier("bookDao1")
    private BookDao bookDao;
    
    public void save() {
        System.out.println("book service save ...");
        bookDao.save();
    }
}
```

@Qualifier 注解后的值就是需要注入的 bean 的名称。

==注意：@Qualifier 不能独立使用，必须和 @Autowired 一起使用==

#### 3.4.4 简单数据类型注入

引用类型看完，简单类型注入就比较容易懂了。简单类型注入的是基本数据类型或者字符串类型，下面在`BookDaoImpl`类中添加一个`name`属性，用其进行简单类型注入

```java
@Repository("bookDao")
public class BookDaoImpl implements BookDao {
    private String name;
    public void save() {
        System.out.println("book dao save ..." + name);
    }
}
```

数据类型换了，对应的注解也要跟着换，这次使用`@Value`注解，将值写入注解的参数中就行了

```java
@Repository("bookDao")
public class BookDaoImpl implements BookDao {
    @Value("itheima")
    private String name;
    public void save() {
        System.out.println("book dao save ..." + name);
    }
}
```

注意数据格式要匹配，如将 "abc" 注入给 int 值，这样程序就会报错。

介绍完后，会有一种感觉就是这个注解好像没什么用，跟直接赋值是一个效果，还没有直接赋值简单，所以这个注解存在的意义是什么?

#### 3.4.5 注解读取properties配置文件

`@Value`一般会被用在从 properties 配置文件中读取内容进行使用，具体如何实现？

##### 步骤1：resource下准备properties文件

jdbc.properties

```properties
name=itheima888
```

##### 步骤2: 使用注解加载properties配置文件

在配置类上添加`@PropertySource`注解

```java
@Configuration
@ComponentScan("com.itheima")
@PropertySource("jdbc.properties")
public class SpringConfig {
}
```

##### 步骤3：使用@Value读取配置文件中的内容

```java
@Repository("bookDao")
public class BookDaoImpl implements BookDao {
    @Value("${name}")
    private String name;
    public void save() {
        System.out.println("book dao save ..." + name);
    }
}
```

步骤4：运行程序

运行App类，查看运行结果，说明配置文件中的内容已经被加载到。

![1630084683663](assets/1630084683663.png)

**注意：**

* 如果读取的 properties 配置文件有多个，可以使用`@PropertySource`的属性来指定多个

  ```java
  @PropertySource({"jdbc.properties", "xxx.properties"})
  ```

* `@PropertySource`注解属性中不支持使用通配符`*`，运行会报错

  ```java
  @PropertySource({"*.properties"})
  ```

* `@PropertySource`注解属性中可以把`classpath:`加上，代表从当前项目的根路径找文件

  ```java
  @PropertySource({"classpath:jdbc.properties"})
  ```

> 通过上述演示，可以发现，我们以后可以把一些配置信息写到配置文件中，再用注解来读出，这样就避免了之前写死在程序中的方式带来的难以维护的问题，但是这里还是有很多同学有疑问，为什么写到配置文件中就好维护了呢？
>
> 1. 配置文件专门用来管理配置信息，比较方便
> 2. 写业务代码，不用考虑具体的配置信息，专注于业务
> 3. 重点：当某个配置需要修改时，直接修改配置文件即可，不需要去修改业务代码，所以业务代码也就不需要重新编译、测试、打包……（当程序体积比较大时，编译测试非常耗时。当程序已经在生成环境部署了，重新编译、测试、打包……简直就是灾难）

#### 知识点1：@Autowired


| 名称 | @Autowired                                                   |
| ---- | ------------------------------------------------------------ |
| 类型 | 属性注解  或  方法注解（了解）  或  方法形参注解（了解）     |
| 位置 | 属性定义上方  或  标准 set 方法上方  或  类set方法上方  或  方法形参前面 |
| 作用 | 为引用类型属性设置值                                         |
| 属性 | required：true/false，定义该属性是否允许为 null              |

#### 知识点2：@Qualifier

| 名称 | @Qualifier                                               |
| ---- | -------------------------------------------------------- |
| 类型 | 属性注解  或  方法注解（了解）                           |
| 位置 | 属性定义上方  或  标准 set 方法上方  或  类 set 方法上方 |
| 作用 | 为引用类型属性指定注入的 beanId                          |
| 属性 | value（默认）：设置注入的 beanId                         |

#### 知识点3：@Value

| 名称 | @Value                                                   |
| ---- | -------------------------------------------------------- |
| 类型 | 属性注解  或  方法注解（了解）                           |
| 位置 | 属性定义上方  或  标准 set 方法上方  或  类 set 方法上方 |
| 作用 | 为  基本数据类型  或  字符串类型  属性设置值             |
| 属性 | value（默认）：要注入的属性值                            |

#### 知识点4：@PropertySource

| 名称 | @PropertySource                                              |
| ---- | ------------------------------------------------------------ |
| 类型 | 类注解                                                       |
| 位置 | 类定义上方                                                   |
| 作用 | 加载 properties 文件中的属性值                               |
| 属性 | value（默认）：设置加载的 properties 文件对应的文件名或文件名组成的数组 |

## 4、IOC/DI注解开发管理第三方bean

前面定义 bean 的时候都是在自己开发的类上面写个注解就完成了，但如果是第三方的类，这些类都是在 jar 包中，我们没有办法在类上面添加注解，这个时候该怎么办？

遇到上述问题，我们就需要有一种更加灵活的方式来定义 bean，这种方式不能在原始代码上面书写注解，一样能定义 bean，这就用到了一个全新的注解==@Bean==。

这个注解该如何使用呢？

咱们把之前使用配置方式管理的数据源使用注解再来一遍，通过这个案例来学习下 @Bean 的使用。

### 4.1 环境准备

学习 @Bean 注解之前先来准备环境：

- 创建一个 Maven 项目

- pom.xml 添加 Spring 的依赖

  ```xml
  <dependencies>
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-context</artifactId>
          <version>5.2.10.RELEASE</version>
      </dependency>
  </dependencies>
  ```

- 添加一个配置类`SpringConfig`

  ```java
  @Configuration
  public class SpringConfig {
  }
  ```

- 添加 BookDao、BookDaoImpl 类

  ```java
  public interface BookDao {
      public void save();
  }
  @Repository
  public class BookDaoImpl implements BookDao {
      public void save() {
          System.out.println("book dao save ..." );
      }
  }
  ```

- 创建运行类 App

  ```java
  public class App {
      public static void main(String[] args) {
          AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
      }
  }
  ```

最终创建好的项目结构如下：

![1630122466404](assets/1630122466404.png)

### 4.2 注解开发管理第三方bean

在上述环境中完成对`Druid`数据源的管理，具体的实现步骤为：

#### 步骤1：导入对应的jar包

```xml
<dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>druid</artifactId>
    <version>1.1.16</version>
</dependency>
```

#### 步骤2：在配置类中添加一个方法

注意该方法的返回值就是要创建的 Bean 对象类型

```java
@Configuration
public class SpringConfig {
    public DataSource dataSource() {
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/spring_db");
        ds.setUsername("root");
        ds.setPassword("root");
        return ds;
    }
}
```

#### 步骤3：在方法上添加`@Bean`注解

@Bean 注解的作用是将方法的返回值制作为 Spring 管理的一个 bean 对象

```java
@Configuration
public class SpringConfig {
	@Bean
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/spring_db");
        ds.setUsername("root");
        ds.setPassword("root");
        return ds;
    }
}
```

**注意：不能使用`DataSource ds = new DruidDataSource()`**

因为 DataSource 接口中没有对应的 setter 方法来设置属性。

#### 步骤4：从IOC容器中获取对象并打印

```java
public class App {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
        DataSource dataSource = ctx.getBean(DataSource.class);
        System.out.println(dataSource);
    }
}
```

至此使用 @Bean 来管理第三方 bean 的案例就已经完成。

如果有多个 bean 要被 Spring 管理，直接在配置类中多些几个方法，方法上添加 @Bean 注解即可。

### 4.3 引入外部配置类

如果把所有的第三方 bean 都配置到 Spring 的配置类`SpringConfig`中，虽然可以，但是不利于代码阅读和分类管理，所有我们就想能不能按照类别将这些 bean 配置到不同的配置类中？

对于数据源的 bean，我们新建一个`JdbcConfig`配置类，并把数据源配置到该类下。

```java
public class JdbcConfig {
	@Bean
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/spring_db");
        ds.setUsername("root");
        ds.setPassword("root");
        return ds;
    }
}
```

现在的问题是，这个配置类如何能被 Spring 配置类加载到，并创建 DataSource 对象在 IOC 容器中？

针对这个问题，有两个解决方案：

#### 4.3.1 使用包扫描引入

##### 步骤1：在Spring的配置类上添加包扫描

```java
@Configuration
@ComponentScan("com.itheima.config")
public class SpringConfig {
	
}
```

##### 步骤2：在JdbcConfig上添加配置注解

JdbcConfig 类要放入到`com.itheima.config`包下，需要被 Spring 的配置类扫描到即可

```java
@Configuration
public class JdbcConfig {
	@Bean
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/spring_db");
        ds.setUsername("root");
        ds.setPassword("root");
        return ds;
    }
}
```

##### 步骤3：运行程序

依然能获取到 bean 对象并打印控制台。

这种方式虽然能够扫描到，但是不能很快的知晓都引入了哪些配置类，所有这种方式不推荐使用。

#### 4.3.2 使用`@Import`引入

方案一实现起来有点小复杂，Spring 早就想到了这一点，于是又给我们提供了第二种方案。

这种方案可以不用加`@Configuration`注解，但是必须在 Spring 配置类上使用`@Import`注解手动引入需要加载的配置类

##### 步骤1：去除JdbcConfig类上的注解

```java
public class JdbcConfig {
	@Bean
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/spring_db");
        ds.setUsername("root");
        ds.setPassword("root");
        return ds;
    }
}
```

##### 步骤2：在Spring配置类中引入

```java
@Configuration
//@ComponentScan("com.itheima.config")
@Import({JdbcConfig.class})
public class SpringConfig {
	
}
```

**注意:**

* 扫描注解可以移除

* @Import 参数需要的是一个**数组**，可以引入多个配置类。

* @Import 注解在配置类中只能写一次，下面的方式是==不允许的==

  ```java
  @Configuration
  // @ComponentScan("com.itheima.config")
  @Import(JdbcConfig.class)
  @Import(Xxx.class)
  public class SpringConfig {
  	
  }
  ```

##### 步骤3：运行程序

依然能获取到 bean 对象并打印控制台

### 知识点1：@Bean

| 名称 | @Bean                                     |
| ---- | ----------------------------------------- |
| 类型 | 方法注解                                  |
| 位置 | 方法定义上方                              |
| 作用 | 设置该方法的返回值作为 spring 管理的 bean |
| 属性 | value（默认）：定义 bean 的 id            |

### 知识点2：@Import

| 名称 | @Import                                                      |
| ---- | ------------------------------------------------------------ |
| 类型 | 类注解                                                       |
| 位置 | 类定义上方                                                   |
| 作用 | 导入配置类                                                   |
| 属性 | value（默认）：定义导入的配置类类名，<br/>当配置类有多个时使用数组格式一次性导入多个配置类 |

### 4.4 注解开发实现为第三方bean注入资源

在使用 @Bean 创建 bean 对象的时候，如果方法在创建的过程中需要其他资源该怎么办？

这些资源会有两大类，分别是`简单数据类型` 和`引用数据类型`。

#### 4.4.1 简单数据类型

##### 4.4.1.1 需求分析

对于下面代码关于数据库的四要素不应该写死在代码中，应该是从 properties 配置文件中读取。如何来优化下面的代码？

```java
public class JdbcConfig {
	@Bean
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/spring_db");
        ds.setUsername("root");
        ds.setPassword("root");
        return ds;
    }
}
```

##### 4.4.1.2 注入简单数据类型步骤

###### 步骤1：类中提供四个属性

```java
public class JdbcConfig {
    private String driver;
    private String url;
    private String userName;
    private String password;
	@Bean
    public DataSource dataSource() {
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/spring_db");
        ds.setUsername("root");
        ds.setPassword("root");
        return ds;
    }
}
```

###### 步骤2：使用`@Value`注解引入值

```java
public class JdbcConfig {
    @Value("com.mysql.jdbc.Driver")
    private String driver;
    @Value("jdbc:mysql://localhost:3306/spring_db")
    private String url;
    @Value("root")
    private String userName;
    @Value("password")
    private String password;
	@Bean
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName(driver);
        ds.setUrl(url);
        ds.setUsername(userName);
        ds.setPassword(password);
        return ds;
    }
}
```

###### 扩展

现在的数据库连接四要素还是写在代码中，需要做的是将这些内容提

取到 jdbc.properties 配置文件，大家思考下该如何实现？

> 1. resources 目录下添加 jdbc.properties
>
> 2. 配置文件中提供四个键值对分别是数据库的四要素
>
> 3. 使用 @PropertySource 加载 jdbc.properties 配置文件
>
> 4. 修改 @Value 注解属性的值，将其修改为`${key}`，key 就是键值对中的键的值

具体的实现就交由大家自行实现下。

#### 4.4.2 引用数据类型

##### 4.4.2.1 需求分析 

假设在构建 DataSource 对象的时候，需要用到 BookDao 对象，该如何把 BookDao 对象注入进方法内让其使用呢？

```java
public class JdbcConfig {
	@Bean
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/spring_db");
        ds.setUsername("root");
        ds.setPassword("root");
        return ds;
    }
}
```

##### 4.4.2.2 注入引用数据类型步骤

###### 步骤1：在SpringConfig中扫描BookDao

扫描的目的是让 Spring 能管理到 BookDao，也就是说要让 IOC 容器中有一个 bookDao 对象

```java
@Configuration
@ComponentScan("com.itheima.dao")
@Import({JdbcConfig.class})
public class SpringConfig {
}
```

###### 步骤2：在JdbcConfig类的方法上添加参数

```java
@Bean
public DataSource dataSource(BookDao bookDao){
    System.out.println(bookDao);
    DruidDataSource ds = new DruidDataSource();
    ds.setDriverClassName(driver);
    ds.setUrl(url);
    ds.setUsername(userName);
    ds.setPassword(password);
    return ds;
}
```

==引用类型注入只需要为 bean 定义方法设置形参即可，容器会根据类型自动装配对象。==

###### 步骤3：运行程序

![1630125475609](assets/1630125475609.png)



## 5、注解开发总结

前面我们已经完成了 XML 配置和注解的开发实现，至于两者之间的差异，咱们放在一块去对比回顾下：

![1630134786448](assets/1630134786448.png)

## 6、Spring整合

课程学习到这里，已经对 Spring 有一个简单的认识了，Spring 有一个容器，叫做 IoC 容器，里面保存 bean。在进行企业级开发的时候，其实除了将自己写的类让 Spring 管理之外，还有一部分重要的工作就是使用第三方的技术。前面已经讲了如何管理第三方 bean 了，下面结合 IoC 和 DI，整合 2 个常用技术，进一步加深对 Spring 的使用理解。

### 6.1 Spring整合Mybatis思路分析

#### 6.1.1 环境准备

在准备环境的过程中，我们也来**回顾**下之前 Mybatis 开发的相关内容

##### 步骤1：准备数据库表

Mybatis 是来操作数据库表，所以先创建一个数据库及表

```sql
create database spring_db character set utf8;
use spring_db;
create table tbl_account(
    id int primary key auto_increment,
    name varchar(35),
    money double
);
```

##### 步骤2：创建项目导入jar包

项目的 pom.xml 添加相关依赖

```xml
<dependencies>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-context</artifactId>
        <version>5.2.10.RELEASE</version>
    </dependency>
    <dependency>
        <groupId>com.alibaba</groupId>
        <artifactId>druid</artifactId>
        <version>1.1.16</version>
    </dependency>
    <dependency>
        <groupId>org.mybatis</groupId>
        <artifactId>mybatis</artifactId>
        <version>3.5.6</version>
    </dependency>
    <dependency>
        <groupId>mysql</groupId>
        <artifactId>mysql-connector-java</artifactId>
        <version>5.1.47</version>
    </dependency>
</dependencies>
```

##### 步骤3：根据表创建模型类

```java
public class Account implements Serializable {

    private Integer id;
    private String name;
    private Double money;
	//s etter...getter...toString...方法略    
}
```

##### 步骤4：创建Dao接口

这里我们用注解配置 SQL 来演示，XML 配置 SQL就不演示了

```java
public interface AccountDao {
    @Insert("insert into tbl_account(name,money)values(#{name}, #{money})")
    void save(Account account);

    @Delete("delete from tbl_account where id = #{id}")
    void delete(Integer id);

    @Update("update tbl_account set name = #{name} , money = #{money} where id = #{id} ")
    void update(Account account);

    @Select("select * from tbl_account")
    List<Account> findAll();

    @Select("select * from tbl_account where id = #{id} ")
    Account findById(Integer id);
}
```

##### 步骤5：创建Service接口和实现类

```java
public interface AccountService {

    void save(Account account);

    void delete(Integer id);

    void update(Account account);

    List<Account> findAll();

    Account findById(Integer id);

}

@Service
public class AccountServiceImpl implements AccountService {

    // 注解方式注入
    @Autowired
    private AccountDao accountDao;

    public void save(Account account) {
        accountDao.save(account);
    }

    public void update(Account account){
        accountDao.update(account);
    }

    public void delete(Integer id) {
        accountDao.delete(id);
    }

    public Account findById(Integer id) {
        return accountDao.findById(id);
    }

    public List<Account> findAll() {
        return accountDao.findAll();
    }
}
```

##### 步骤6：添加jdbc.properties文件

resources 目录下添加，用于配置数据库连接四要素

```properties
jdbc.driver=com.mysql.jdbc.Driver
jdbc.url=jdbc:mysql://localhost:3306/spring_db?useSSL=false
jdbc.username=root
jdbc.password=root
```

useSSL：关闭 MySQL 的 SSL 连接

##### 步骤7：添加Mybatis核心配置文件

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE configuration
        PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <!-- 读取外部properties配置文件 -->
    <properties resource="jdbc.properties"></properties>
    <!-- 别名扫描的包路径 -->
    <typeAliases>
        <package name="com.itheima.domain"/>
    </typeAliases>
    <!-- 数据源 -->
    <environments default="mysql">
        <environment id="mysql">
            <transactionManager type="JDBC"></transactionManager>
            <dataSource type="POOLED">
                <property name="driver" value="${jdbc.driver}"></property>
                <property name="url" value="${jdbc.url}"></property>
                <property name="username" value="${jdbc.username}"></property>
                <property name="password" value="${jdbc.password}"></property>
            </dataSource>
        </environment>
    </environments>
    <!-- 映射文件扫描包路径 -->
    <mappers>
        <package name="com.itheima.dao"></package>
    </mappers>
</configuration>
```

##### 步骤8：编写应用程序

```java
public class App {
    public static void main(String[] args) throws IOException {
        // 1. 创建 SqlSessionFactoryBuilder 对象
        SqlSessionFactoryBuilder sqlSessionFactoryBuilder = new SqlSessionFactoryBuilder();
        // 2. 加载 SqlMapConfig.xml 配置文件
        InputStream inputStream = Resources.getResourceAsStream("SqlMapConfig.xml.bak");
        // 3. 创建 SqlSessionFactory 对象
        SqlSessionFactory sqlSessionFactory = sqlSessionFactoryBuilder.build(inputStream);
        // 4. 获取 SqlSession
        SqlSession sqlSession = sqlSessionFactory.openSession();
        // 5. 执行 SqlSession 对象执行查询，获取结果 User
        AccountDao accountDao = sqlSession.getMapper(AccountDao.class);

        Account ac = accountDao.findById(1);
        System.out.println(ac);

        // 6. 释放资源
        sqlSession.close();
    }
}
```

##### 步骤9：运行程序

![1630136904087](assets/1630136904087.png)

#### 6.1.2 整合思路分析

Mybatis 的基础环境我们已经准备好了，接下来就得分析下在上述的内容中，哪些对象可以交给 Spring 来管理？

* Mybatis 程序核心对象分析

  ![1630137189480](assets/1630137189480.png)

  从图中可以获取到，真正需要交给 Spring 管理的是==SqlSessionFactory==

* 整合 Mybatis，就是将 Mybatis 用到的内容交给 Spring 管理，分析下配置文件

  ![1630137388717](assets/1630137388717.png)

  **说明：**

  * 第一行读取外部 properties 配置文件，Spring 有提供具体的解决方案`@PropertySource`，需要交给 Spring
  * 第二行起别名包扫描，为 SqlSessionFactory 服务的，需要交给 Spring
  * 第三行主要用于做连接池，Spring 之前我们已经整合了 Druid 连接池，这块也需要交给 Spring
  * 前面三行一起都是为了创建 SqlSession 对象用的，那么用 Spring 管理 SqlSession 对象吗？回忆下 SqlSession 是由SqlSessionFactory创建出来的，所以只需要将 SqlSessionFactory 交给 Spring 管理即可。
  * 第四行是 Mapper 接口和映射文件[如果使用注解就没有该映射文件]，这个是在获取到 SqlSession 以后执行具体操作的时候用，所以它和 SqlSessionFactory 创建的时机都不在同一个时间，可能需要单独管理。

### 6.2 Spring整合Mybatis

前面我们已经分析了 Spring 与 Mybatis 的整合，大体需要做两件事

第一件事是：Spring 要管理 MyBatis 中的 SqlSessionFactory

第二件事是：Spring 要管理 Mapper 接口的扫描

具体该如何实现，具体的步骤为：

#### 步骤1：项目中导入整合需要的jar包

```xml
<dependency>
    <!-- Spring 操作数据库需要该 jar 包 -->
    <groupId>org.springframework</groupId>
    <artifactId>spring-jdbc</artifactId>
    <version>5.2.10.RELEASE</version>
</dependency>
<dependency>
    <!--
		Spring 与 Mybatis 整合的 jar 包
		这个 jar 包 mybatis 在前面，是 Mybatis 提供的
	-->
    <groupId>org.mybatis</groupId>
    <artifactId>mybatis-spring</artifactId>
    <version>1.3.0</version>
</dependency>
```

#### 步骤2：创建Spring的主配置类

```java
@Configuration
@ComponentScan("com.itheima")
public class SpringConfig {
}
```

#### 步骤3：创建数据源的配置类

在配置类中完成数据源的创建

```java
public class JdbcConfig {
    @Value("${jdbc.driver}")
    private String driver;
    @Value("${jdbc.url}")
    private String url;
    @Value("${jdbc.username}")
    private String userName;
    @Value("${jdbc.password}")
    private String password;

    @Bean
    public DataSource dataSource() {
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName(driver);
        ds.setUrl(url);
        ds.setUsername(userName);
        ds.setPassword(password);
        return ds;
    }
}
```

> jdbc.properties：
>
> ```properties
> jdbc.driver=com.mysql.jdbc.Driver
> jdbc.url=jdbc:mysql://localhost:3306/spring_db?useSSL=false
> jdbc.username=root
> jdbc.password=root
> ```

#### 步骤4：主配置类中读properties并引入数据源配置类

```java
@Configuration
@ComponentScan("com.itheima")
@PropertySource("classpath:jdbc.properties")
@Import(JdbcConfig.class)
public class SpringConfig {
}
```

#### 步骤5：创建Mybatis配置类并配置SqlSessionFactory

```java
public class MybatisConfig {
    // 定义 bean，MyBatis-Spring 提供 SqlSessionFactoryBean 用于产生 SqlSessionFactory 对象
    // 注解定义的 Bean 注入引用类型需要传递一个参数
    // 这里我们需要注入 JdbcConfig 中的 DataSource bean
    @Bean
    public SqlSessionFactoryBean sqlSessionFactory(DataSource dataSource) {
        SqlSessionFactoryBean ssfb = new SqlSessionFactoryBean();
        // 别名扫描的包路径
        ssfb.setTypeAliasesPackage("com.itheima.domain");
        // 设置数据源
        ssfb.setDataSource(dataSource);
        return ssfb;
    }
    
    // 定义 bean，返回 MapperScannerConfigurer 对象
    @Bean
    public MapperScannerConfigurer mapperScannerConfigurer() {
        MapperScannerConfigurer msc = new MapperScannerConfigurer();
        msc.setBasePackage("com.itheima.dao");
        return msc;
    }
}
```

**说明：**

* 使用 SqlSessionFactoryBean 封装 SqlSessionFactory 需要的环境信息

  ![1630138835057](assets/1630138835057.png)

  * SqlSessionFactoryBean 是前面我们讲解 FactoryBean 的一个子类，在该类中将 SqlSessionFactory 的创建进行了封装，简化对象的创建，我们只需要将其需要的内容设置即可。
  * 方法中有一个参数为 dataSource，当前 Spring 容器中已经创建了 Druid 数据源，类型刚好是 DataSource 类型，此时在初始化 SqlSessionFactoryBean 这个对象的时候，发现需要使用 DataSource 对象，而容器中刚好有这么一个对象，就自动加载了 DruidDataSource 对象。

* 使用 MapperScannerConfigurer 加载 Dao 接口，创建代理对象保存到 IOC 容器中

  ![1630138916939](assets/1630138916939.png)

  * 这个 MapperScannerConfigurer 对象也是 MyBatis 提供的专用于整合的 jar 包中的类，用来处理原始配置文件中的 mappers 相关配置，加载数据层的 Mapper 接口类
  * MapperScannerConfigurer 有一个核心属性 basePackage，就是用来设置所扫描的包路径

#### 步骤6：主配置类中引入Mybatis配置类

```java
@Configuration
@ComponentScan("com.itheima")
@PropertySource("classpath:jdbc.properties")
@Import({JdbcConfig.class, MybatisConfig.class})
public class SpringConfig {
}
```

#### 步骤7：编写运行类

在运行类中，从 IOC 容器中获取 Service 对象，调用方法获取结果

```java
public class App2 {
    public static void main(String[] args) {
        ApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
        AccountService accountService = ctx.getBean(AccountService.class);
        Account ac = accountService.findById(1);
        System.out.println(ac);
    }
}
```

#### 步骤8：运行程序

![1630139036627](assets/1630139036627.png)

支持 Spring 与 Mybatis 的整合就已经完成了，其中主要用到的两个类分别是：

* ==SqlSessionFactoryBean==
* ==MapperScannerConfigurer==

### 6.3 Spring整合Junit

整合 Junit 与整合 Druid 和 MyBatis 差异比较大，为什么呢？Junit 是一个搞单元测试用的工具，它不是我们程序的主体，也不会参加最终程序的运行，从作用上来说就和之前的东西不一样，它不是做功能的，看做是一个辅助工具就可以了。

#### 6.3.1 环境准备

这块环境，大家可以直接使用 Spring 与 Mybatis 整合的环境即可。当然也可以重新创建一个，因为内容是一模一样，所以我们直接来看下项目结构即可：

![1630139720273](assets/1630139720273.png)

#### 6.3.2 整合Junit步骤

在上述环境的基础上，我们来对 Junit 进行整合。

##### 步骤1：引入依赖

pom.xml

```xml
<dependency>
    <groupId>junit</groupId>
    <artifactId>junit</artifactId>
    <version>4.12</version>
    <scope>test</scope>
</dependency>

<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-test</artifactId>
    <version>5.2.10.RELEASE</version>
</dependency>
```

##### 步骤2：编写测试类

在 test\java 下创建一个 AccountServiceTest，这个名字任意

```java
// 设置类运行器
@RunWith(SpringJUnit4ClassRunner.class)
// 设置Spring环境对应的配置类
@ContextConfiguration(classes = {SpringConfiguration.class}) // 加载配置类
// @ContextConfiguration(locations={"classpath:applicationContext.xml"}) // 加载配置文件
public class AccountServiceTest {
    // 自动装配注入bean
    @Autowired
    private AccountService accountService;
    @Test
    public void testFindById() {
        System.out.println(accountService.findById(1));
    }
    @Test
    public void testFindAll() {
        System.out.println(accountService.findAll());
    }
}
```

**注意:**

* 单元测试，如果测试的是注解配置类，则使用`@ContextConfiguration(classes = 配置类.class)`
* 单元测试，如果测试的是配置文件，则使用`@ContextConfiguration(locations={配置文件名,...})`
* Junit 运行后是基于 Spring 环境运行的，所以 Spring 提供了一个专用的类运行器，这个务必要设置，这个类运行器就在 Spring 的测试专用包中提供的，导入的坐标就是这个东西`SpringJUnit4ClassRunner`
* 上面两个配置都是固定格式，当需要测试哪个 bean 时，使用自动装配加载对应的对象，下面的工作就和以前做 Junit 单元测试完全一样了

#### 知识点1：@RunWith

| 名称 | @RunWith                          |
| ---- | --------------------------------- |
| 类型 | 测试类注解                        |
| 位置 | 测试类定义上方                    |
| 作用 | 设置 JUnit 运行器                 |
| 属性 | value（默认）：运行所使用的运行期 |

#### 知识点2：@ContextConfiguration

| 名称 | @ContextConfiguration                                        |
| ---- | ------------------------------------------------------------ |
| 类型 | 测试类注解                                                   |
| 位置 | 测试类定义上方                                               |
| 作用 | 设置 JUnit 加载的 Spring 核心配置                            |
| 属性 | classes：核心配置类，可以使用数组的格式设定加载多个配置类<br/>locations：配置文件，可以使用数组的格式设定加载多个配置文件名称 |

