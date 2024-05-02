# Creating A New Layer & New Recipe

## 1. To create the layer
* general command to create a new layer
  
`bitbake-layers create-layer PathToAddLayerIn/"layername"`

### in our case
```
source oe-init-build-env

bitbake-layers create-layer ./meta-mylayer
```
## 2. Now we need to add our new layer to bblayers.conf
```
cd build/

bitbake-layers add-layer ../meta-mylayer/

````
### you can check if it was added successfully using
`bitbake-layers show-layers `


## 3. Go inside the layer and create a directory to hold the recipe
```
cd meta-mylayer/recipes-example/

mkdir demoapp

cd demoapp/

touch demoapp_1.0.bb
```

## 4. Now if you try to bitbake the attached recipe it would give you an error 
to solve this error we need to create a patch to edit the used Cmake version