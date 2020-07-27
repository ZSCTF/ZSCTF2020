# Race

## 题目名称

race

## 分类

* pwn

## 分数

50

## flag

```txt
zsctf{simp13-Race-C0ndition} 
```

## develop

```bash
# 测试环境
nc cloudtest.zhakul.top 1000  
```

## 题目搭建

```bash
cd pwn/rece/docker && docker-compose up -d
```

> 修改 docker-compose.yml 环境变量可改变 flag。